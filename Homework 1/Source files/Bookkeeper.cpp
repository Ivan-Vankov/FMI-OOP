#include "Bookkeeper.h"

const unsigned Bookkeeper::systemID = 4294967295;
const unsigned Bookkeeper::maxNameLength = 255;
const double Bookkeeper::fmiCoinValue = 375;

Bookkeeper::Bookkeeper(){
	 loadExistingWallets();
	 loadExistingOrders();
}

Bookkeeper::Bookkeeper(const Bookkeeper& other){
	copyFrom(other);
}

Bookkeeper& Bookkeeper::operator=(const Bookkeeper& other){
	if (this != &other){
		copyFrom(other);
	}
	return *this;
}

Bookkeeper::~Bookkeeper(){
	// No dynamic data => no need to delete anything
}


void Bookkeeper::copyFrom(const Bookkeeper& other){
	wallets = other.wallets;
	orders = other.orders;
}

// Default constructor is here so that it can be used in a list
Bookkeeper::Transaction::Transaction() :
	transactionTime(time(NULL)),
	senderId(systemID),
	receiverId(systemID),
	fmiCoins(0){
}

Bookkeeper::Transaction::Transaction(unsigned newSenderId, unsigned newReceiverId, 
	double newFmiCoins, time_t newTransactionTime) :
	transactionTime(newTransactionTime),
	senderId(newSenderId),
	receiverId(newReceiverId),
	fmiCoins(newFmiCoins){
}

Bookkeeper::Transaction::Transaction(const Transaction& other){
	copyFrom(other);
}

Bookkeeper::Transaction& Bookkeeper::Transaction::operator=(const Transaction& other){
	if (this != &other){
		// No dynamic data => no need to delete anything
		copyFrom(other);
	}
	return *this;
}

Bookkeeper::Transaction::~Transaction(){
	// No dynamic data => no need to delete anything
}

void Bookkeeper::Transaction::copyFrom(const Transaction& other){
	transactionTime = other.transactionTime;
	senderId = other.senderId;
	receiverId = other.receiverId;
	fmiCoins = other.fmiCoins;
}

// Default constructor is here so that it can be used in a list
Bookkeeper::Order::Order() :
	type(BUY),
	walletId(systemID),
	fmiCoins(0){
}

Bookkeeper::Order::Order(OrderType newType, unsigned newWalletId, double newFmiCoins) :
	type(newType),
	walletId(newWalletId),
	fmiCoins(newFmiCoins){
}

Bookkeeper::Order::Order(const Order& other){
	copyFrom(other);
}

Bookkeeper::Order& Bookkeeper::Order::operator=(const Order& other){
	if (this != &other){
		// No dynamic data => no need to delete anything
		copyFrom(other);
	}
	return *this;
}

Bookkeeper::Order::~Order(){
	// No dynamic data => no need to delete anything
}

void Bookkeeper::Order::copyFrom(const Order& other){
	type = other.type;
	walletId = other.walletId;
	fmiCoins = other.fmiCoins;
}

void Bookkeeper::addWallet(double fiatMoney, const String& name){
	try{
		String actualName(name);
		// Check if the given name is longer than the allowed amount
		if (name.getLength() > maxNameLength){
			std::cerr << "Can't have a name longer than 255 symbols.";
			// If it is then we cut it to fit
			actualName.cutAfter(maxNameLength);
		}
		// Check if there is already a wallet under that name
		if (walletOwnerIndex(actualName) != -1){
			throw std::exception("A wallet under that name has already been opened.");
		}
		wallets.add(std::make_pair(actualName, fiatMoney));
		// Add a transaction from the systemID to the current wallet for fiatMoney/fmiCoinValue in fmiCoins
		saveTransactionToFile(systemID, (unsigned)wallets.getSize() - 1, fiatMoney / fmiCoinValue);
	}
	catch (std::exception&){
		throw;
	}
}

void Bookkeeper::makeOrder(OrderType orderType, double fmiCoins, unsigned walletID){
	try{
		// Check for walletID legitimacy
		unsigned walletsNumber = (unsigned)wallets.getSize();
		if (!walletsNumber || walletsNumber <= walletID){
			throw std::exception("Invalid walletID.");
		}
		if (orderType == SELL){
			makeSellOrder(fmiCoins, walletID);
		}
		else{
			makeBuyOrder(fmiCoins, walletID);
		}
	}
	catch (std::exception&){
		throw;
	}
}

void Bookkeeper::makeSellOrder(double sellerFmiCoins, unsigned sellerWalletID){
	try{
		double fmiCoinsInWallet = fmiCoinsUnder(sellerWalletID);
		// Check if the person is trying to sell more coins than he has
		if (sellerFmiCoins > fmiCoinsInWallet){
			throw std::exception("Can't sell more coins than you have.");
		}
		// 1) No active orders check
		// 2) At any one time if orders exist then they are either only buy orders or only sell orders
		// So if the first order is of a sell type then there can't be any buy orders from which to buy coins
		// So we just add the current order at the back of all orders
		if (!orders.getSize() || orders.front().type == SELL){
			Order newOrder(SELL, sellerWalletID, sellerFmiCoins);
			orders.pushBack(newOrder);
			return;
		}

		String textFileName(wallets[sellerWalletID].first);
		char buff[20];
		_itoa_s((int)time(NULL), buff, 10);
		textFileName + buff + ".txt";
		// Text log of the transactions that result from the current order
		std::ofstream textFile(textFileName);
		if (!textFile.is_open()){
			throw std::exception("Couldn't open file.");
		}

		// These will be added at the ent of the text file
		int transactionCounter = 0;
		double totalTransactionValue = 0;

		// Now we are guaranteed that orders exist and that ther are only buy ones  
		List<Order>::Iterator i = orders.begin();
		while (i != orders.end()){
			double buyerFmiCoins = (*i).fmiCoins;
			unsigned buyerWalletID = (*i).walletId;
			double smallerFmiCoinsAmmount = buyerFmiCoins < sellerFmiCoins ? buyerFmiCoins : sellerFmiCoins;
			// FiatMoney exchange
			double fiatMoneyExchanged = smallerFmiCoinsAmmount*fmiCoinValue;
			wallets[buyerWalletID].second -= fiatMoneyExchanged;
			wallets[sellerWalletID].second += fiatMoneyExchanged;

			totalTransactionValue += fiatMoneyExchanged;
			// FmiCoins exchange
			saveTransactionToFile(sellerWalletID, buyerWalletID, smallerFmiCoinsAmmount);

			++transactionCounter;
			// Record the exchange in the text file
			textFile << "<seller> " << wallets[sellerWalletID].first << "	<buyer> " << wallets[buyerWalletID].first
				<< "	<coins transfered> " << smallerFmiCoinsAmmount << "\n";
			if (!textFile.good()){
				throw std::exception("Couldn't write to file.");
			}

			if (buyerFmiCoins <= sellerFmiCoins){
				sellerFmiCoins -= buyerFmiCoins;
				++i;
				orders.popFront();
			}
			else{
				sellerFmiCoins = 0;
				break;
			}
		}
		assert(sellerFmiCoins >= 0);
		if (sellerFmiCoins > 0){
			Order newOrder(SELL, sellerWalletID, sellerFmiCoins);
			orders.pushBack(newOrder);
		}

		// Add the number of transactions and total transaction value to the text file
		textFile << "<total transactions> " << transactionCounter
			<< "	<total value> " << totalTransactionValue << " fmiCoins.";
		if (!textFile.good()){
			throw std::exception("Couldn't write to file.");
		}
	}
	catch (std::exception&){
		throw;
	}
}

void Bookkeeper::makeBuyOrder(double buyerFmiCoins, unsigned buyerWalletID){
	try{
		double moneyInBuyerWallet = wallets[buyerWalletID].second;
		// Check if the buyer has enough money to pay for the fmiCoins
		if (buyerFmiCoins > moneyInBuyerWallet / fmiCoinValue){
			throw std::exception("Not enough money in wallet to make that buy order.");
		}

		// 1) No active orders check
		// 2) At any one time if orders exist then they are either only buy orders or only sell orders
		// So if the first order is of a buy type
		// We just add the current order at the back of all orders
		if (!orders.getSize() || orders.front().type == BUY){
			Order newOrder(BUY, buyerWalletID, buyerFmiCoins);
			orders.pushBack(newOrder);
			return;
		}

		String textFileName(wallets[buyerWalletID].first);
		char buff[20];
		_itoa_s((int)time(NULL), buff, 10);
		textFileName + buff + ".txt";
		// Text log of the transactions that result from the current order
		std::ofstream textFile(textFileName);
		if (!textFile.is_open()){
			throw std::exception("Couldn't open file.");
		}

		// These will be added at the ent of the text file
		int transactionCounter = 0;
		double totalTransactionValue = 0;

		// Now we are guaranteed that orders exist and that ther are only buy ones  
		List<Order>::Iterator i = orders.begin();
		while (i != orders.end()){
			double sellerFmiCoins = (*i).fmiCoins;
			unsigned sellerID = (*i).walletId;
			double smallerFmiCoinsAmmount = buyerFmiCoins < sellerFmiCoins ? buyerFmiCoins : sellerFmiCoins;
			// FiatMoney exchange
			double fiatMoneyExchanged = smallerFmiCoinsAmmount*fmiCoinValue;
			wallets[buyerWalletID].second -= fiatMoneyExchanged;
			wallets[sellerID].second += fiatMoneyExchanged;

			totalTransactionValue += fiatMoneyExchanged;
			// FmiCoins exchange
			saveTransactionToFile(sellerID, buyerWalletID, smallerFmiCoinsAmmount);

			++transactionCounter;
			// Record the exchange in the text file
			textFile << "<seller> " << wallets[sellerID].first << "	<buyer> " << wallets[buyerWalletID].first
				<< "	<coins transfered> " << smallerFmiCoinsAmmount << "\n";
			if (!textFile.good()){
				throw std::exception("Couldn't write to file.");
			}

			if (smallerFmiCoinsAmmount <= buyerFmiCoins){
				buyerFmiCoins -= smallerFmiCoinsAmmount;
				++i;
				orders.popFront();
			}
			else{
				buyerFmiCoins = 0;
				break;
			}
		}
		assert(buyerFmiCoins >= 0);
		if (buyerFmiCoins > 0){
			Order newOrder(BUY, buyerWalletID, buyerFmiCoins);
			orders.pushBack(newOrder);
		}

		// Add the number of transactions and total transaction value to the text file
		textFile << "<total transactions> " << transactionCounter 
			<< "	<total value> " << totalTransactionValue << " fmiCoins.";
		if (!textFile.good()){
			throw std::exception("Couldn't write to file.");
		}
	}
	catch (std::exception&){
		throw;
	}
}

void Bookkeeper::saveTransactionToFile(unsigned newSenderId, unsigned newReceiverId,
	double newFmiCoins){
	try{
		std::ofstream ofile("transactions.dat", std::ios::binary | std::ios::app);
		if (!ofile.is_open()){
			throw std::exception("Couldn't open file <transactions.dat> for writing.");
		}
		Transaction currTransaction(newSenderId, newReceiverId, newFmiCoins);
		ofile.write((char*)&currTransaction, sizeof(Transaction));
		if (!ofile.good()){
			throw std::exception("Couldn't write to file <transactions.dat>.");
		}
		ofile.close();
	}
	catch (std::exception&){
		throw;
	}
}

void Bookkeeper::walletInfo(unsigned walletID)const{
	unsigned walletsNumber = (unsigned)wallets.getSize();
	if (walletID == systemID){
		std::cout << "SystemID" << " fiatMoney: Unlimited " << "fmiCoins: Unlimited" << std::endl;
		return;
	}
	if (!walletsNumber){
		std::cout << "No opened wallets." << std::endl;
		return;
	}
	if (walletID >= walletsNumber){
		std::cout << "No wallet with ID <" << walletID << "> exists." << std::endl;
		return;
	}
	std::cout << wallets[walletID].first << " fiatMoney: " << wallets[walletID].second <<
		" fmiCoins: " << fmiCoinsUnder(walletID) << std::endl;
}

void Bookkeeper::attractInvestors()const{
	int walletCount = wallets.getSize();
	if (!walletCount){
		std::cout << "No users for now." << std::endl;
		return;
	}

	// A buffer containing the fmiCoins under all wallets
	Vector<double> buff(walletCount, 0);

	std::ifstream ifile("transactions.dat", std::ios::binary);
	if (!ifile.is_open()){
		throw std::exception("Couldn't open <transaction.dat> for reading.");
	}
	Transaction dummyTransaction;
	// Read through all transactions
	while (ifile.read((char*)&dummyTransaction, sizeof(Transaction))){
		// If the sender or receiver IDs aren the systemID
		// subtract the fmiCoins from the current transaction from the seller
		// and add them to the buyer
		if (dummyTransaction.senderId != systemID){
			buff[dummyTransaction.senderId] -= dummyTransaction.fmiCoins;
		}
		if (dummyTransaction.receiverId != systemID){
			buff[dummyTransaction.receiverId] += dummyTransaction.fmiCoins;
		}
	}
	if (!ifile.eof()){
		throw std::exception("Couldn't read from file <transactions.dat>.");
	}
	ifile.close();
	ifile.clear();

	int userCnt = 10 < walletCount ? 10 : walletCount;

	// Make a buffer for the richest users
	// The double in the pair is the fmiCoins
	// The int is the id of the user
	Vector<std::pair<double, int>> bestUsers(userCnt);
	// Index for the currently richest user
	int currRichest = 0;
	// Fill the buffer
	for (int i = 0; i < userCnt; ++i, currRichest = 0){
		// Go through the fmiCoins buffer and find the richest
		for (int j = 1; j < buff.getSize(); ++j){
			// If a richer user is found
			// Make him the richest
			if (buff[j]>buff[currRichest]){
				currRichest = j;
			}
		}
		// Add the currentRichest to the users buffer
		bestUsers[i] = std::make_pair(buff[currRichest], currRichest);
		buff[currRichest] = -1;
	}

	// We no longer need the fmiCoins buffer
	buff.clean();
	
	// Now we read the number of transaction under the top users
	// and the times of their first and last transaction

	// Buffer for the first and last transactions
	Vector<std::pair<time_t, time_t>> firstAndLastTrans(userCnt, std::make_pair(0,0));
	// Buffer for the number of transactions
	Vector<int> transactionsCnt(userCnt, 0);
	ifile.open("transactions.dat", std::ios::binary);
	// Read through all transactions
	while (ifile.read((char*)&dummyTransaction, sizeof(Transaction))){
		// Try to find if the senderID and/or the receiverID are among the richest 
		int senderIndex = -1;
		int receiverIndex = -1;
		for (int i = 0; i < userCnt; ++i){
			// If the senderId is the i-th richest user
			if (bestUsers[i].second == dummyTransaction.senderId){
				// Save that index
				senderIndex = i;
			}
			// If the receiverId is the i-th richest user
			if (bestUsers[i].second == dummyTransaction.receiverId){
				// Save that index
				receiverIndex = bestUsers[i].second;
			}
			if (senderIndex != -1 && receiverIndex != -1){
				break;
			}
		}
		// If the senderID is amongst the the richest users
		if (senderIndex != -1){
			// Increase the number of transactions done by the sender
			++transactionsCnt[senderIndex];
			// If there is no first transaction yet
			if (!firstAndLastTrans[senderIndex].first){
				// Add it
				firstAndLastTrans[senderIndex].first = dummyTransaction.transactionTime;
			}
			// And update the last transaction
			firstAndLastTrans[senderIndex].second = dummyTransaction.transactionTime;
		}
		// If the receiverID is amongst the the richest users
		if (receiverIndex != -1){
			// Increase the number of transactions done by the receiver
			++transactionsCnt[receiverIndex];
			// If there is no first transaction yet
			if (!firstAndLastTrans[receiverIndex].first){
				// Add it
				firstAndLastTrans[receiverIndex].first = dummyTransaction.transactionTime;
			}
			// And update the last transaction
			firstAndLastTrans[receiverIndex].second = dummyTransaction.transactionTime;
		}
	}
	if (!ifile.eof()){
		throw std::exception("Couldn't read from file <transactions.dat>.");
	}
	ifile.close();

	// Now we just print the richest users
	for (int i = 0; i < userCnt; ++i){
		std::cout << "User: " << wallets[bestUsers[i].second].first << std::endl;
		std::cout << "FmiCoins: " << bestUsers[i].first << std::endl;
		std::cout << "Number of transactions: " << transactionsCnt[i] << std::endl;
		std::cout << "Time of first transaction: " << firstAndLastTrans[i].first << std::endl;
		std::cout << "Time of last transaction: " << firstAndLastTrans[i].second << std::endl << std::endl;
	}
}

// Searches through all transactions to find the number of fmiCoins under walletID
// also it has no checks for correctness of walletID because it is a private method
// and only correct walletIDs will be passed to it
double Bookkeeper::fmiCoinsUnder(unsigned walletID)const{
	try{
		double fmiCoins = 0;
		std::ifstream ifile("transactions.dat", std::ios::binary);
		if (!ifile.is_open()){
			throw std::exception("Couldn't open <transaction.dat> for reading.");
		}
		Transaction dummyTransaction;
		while (ifile.read((char*)&dummyTransaction, sizeof(Transaction))){
			if (dummyTransaction.senderId == walletID){
				fmiCoins -= dummyTransaction.fmiCoins;
			}
			if (dummyTransaction.receiverId == walletID){
				fmiCoins += dummyTransaction.fmiCoins;
			}
		}
		if (!ifile.eof()){
			throw std::exception("Couldn't read from file <transactions.dat>.");
		}
		ifile.close();

		return fmiCoins;
	}
	catch(std::exception&){
		throw;
	}
}

int Bookkeeper::walletOwnerIndex(const String& name)const{
	for (int i = 0; i < wallets.getSize(); ++i){
		if (name == wallets[i].first){
			return i;
		}
	}
	return -1;
}

// Returns the number of opened wallets
unsigned Bookkeeper::getLastWalletID(){
	return (unsigned)wallets.getSize();
}

void Bookkeeper::loadExistingWallets(){
	try{
		if (!fileExists("wallets.dat")){
			createFile("wallets.dat");
			return;
		}
		std::ifstream ifile("wallets.dat", std::ios::binary);
		if (!ifile.is_open()){
			throw std::exception("Couldn't open <wallets.dat>.");
		}
		char c;
		std::pair<String, double> currWallet;
		// This way we check if there is another wallet to load
		while (ifile.read(&c, sizeof(char))){
			currWallet.first.add(c);
			// First load the name
			while (ifile.read(&c, sizeof(char))){
				if (!ifile.good()){
					if (ifile.eof()){
						break;
					}
					throw std::exception("Couldn't read from file <wallets.dat>.");
				}
				if (c == ' ' || c == '\0'){
					break;
				}
				currWallet.first.add(c);
			}

			// Then we load the fiatMoney
			ifile.read((char*)&currWallet.second, sizeof(double));
			if (!ifile.good()){
				throw std::exception("Couldn't read from file <wallets.dat>.");
			}
			wallets.add(currWallet);
			currWallet.first.free();
		}
		// Check if the whole file hasn't been read
		if (!ifile.eof()){
			throw std::exception("Couldn't read from file <wallets.dat>.");
		}
		ifile.close();
	}
	catch (std::exception&){
		throw;
	}
}

void Bookkeeper::loadExistingOrders(){
	try{
		if (!fileExists("orders.dat")){
			createFile("orders.dat");
			return;
		}
		std::ifstream ifile("orders.dat", std::ios::binary);
		if (!ifile.is_open()){
			throw std::exception("Couldn't open <orders.dat>.");
		}
		char c;
		// Check for an empty file
		ifile.read(&c, sizeof(char));
		if (!ifile.good()){
			if (ifile.eof()){
				ifile.close();
				return;
			}
			else{
				throw std::exception("Couldn't read from file <orders.dat>.");
			}
		}
		Order currOrder;
		if (c == 's'){
			currOrder.type = SELL;
		}
		else{
			currOrder.type = BUY;
		}
		unsigned dummyWalletID;
		while (ifile.read((char*)&dummyWalletID, sizeof(unsigned))){
			// If there is a wallet ID then add it to the current order
			currOrder.walletId = dummyWalletID;

			// Read the receiverID
			ifile.read((char*)&currOrder.fmiCoins, sizeof(double));
			if (!ifile.good()){
				throw std::exception("Couldn't read from file <orders.dat>.");
			}

			orders.pushBack(currOrder);
		}
		// Check if the whole file hasn't been read
		if (!ifile.eof()){
			throw std::exception("Couldn't read from file <orders.dat>.");
		}
		ifile.close();
	}
	catch (std::exception&){
		throw;
	}
}

void Bookkeeper::saveToFile(){
	try{
		saveWalletsToFile();
		saveOrdersToFile();
	}
	catch (std::exception&){
		throw;
	}
}

void Bookkeeper::saveWalletsToFile()const{
	try{
		std::ofstream ofile("wallets.dat", std::ios::binary | std::ios::trunc);
		if (!ofile.is_open()){
			throw std::exception("Couldn't open file <wallets.dat> for writing.");
		}
		// For all newly opened wallets
		for (unsigned i = 0; i < (unsigned)wallets.getSize(); ++i){
			// Save the name
			//				 <= to catch the '\0'
			for (int j = 0; j <= wallets[i].first.getLength(); ++j){
				// Have to use c because wallets[i].first[j] is not an lvalue and therefore can't have its adress taken
				char c = wallets[i].first[j];
				ofile.write(&c, sizeof(char));
				if (!ofile.good()){
					throw std::exception("Couldn't write to file <wallets.dat>.");
				}
			}
			// And save the fiatMoney
			ofile.write((char*)&wallets[i].second, sizeof(double));
			if (!ofile.good()){
				throw std::exception("Couldn't write to file <wallets.dat>.");
			}
		}
		ofile.close();
	}
	catch (std::exception&){
		throw;
	}
}

void Bookkeeper::saveOrdersToFile()const{
	try{
		std::ofstream ofile("orders.dat", std::ios::binary | std::ios::trunc);
		if (!ofile.is_open()){
			throw std::exception("Couldn't open file <orders.dat> for writing.");
		}
		char c;
		// Write nothing if there is nothing to write
		if (orders.isEmpty()){
			return;
		}
		// Get the order type
		else if (orders.front().type == SELL){
			c = 's';
		}
		else{
			c = 'b';
		}
		ofile.write(&c, sizeof(char));
		if (!ofile.good()){
			throw std::exception("Couldn't write to file <orders.dat>.");
		}
		// For all orders
		for (List<Order>::constIterator i = orders.begin(); i != orders.end(); ++i){
			// Save the walletID
			ofile.write((char*)&(*i).walletId, sizeof(unsigned));
			if (!ofile.good()){
				throw std::exception("Couldn't write to file <orders.dat>.");
			}
			// Save the fmiCoins
			ofile.write((char*)&(*i).fmiCoins, sizeof(double));
			if (!ofile.good()){
				throw std::exception("Couldn't write to file <orders.dat>.");
			}
		}
		ofile.close();
	}
	catch (std::exception&){
		throw;
	}
}

void Bookkeeper::createFile(const char* fileName)const{
	std::ofstream ofile(fileName/*, std::ios::binary*/);
	try{
		if (!ofile.good()){
			String except = "Couldn't create file <";
			except + fileName + ">.";
			throw std::exception(except);
		}
	}
	catch (std::exception&){
		throw;
	}
}

bool Bookkeeper::fileExists(const char* fileName)const{
	bool fileOpenedSuccessfully;
	std::ifstream ifile(fileName);
	fileOpenedSuccessfully = ifile.is_open();
	ifile.close();
	return fileOpenedSuccessfully;
}
