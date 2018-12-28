#ifndef _BOOKKEEPER_HEADER_
#define _BOOKKEEPER_HEADER_
#include "String.h"
#include "Vector.h"
#include "List.h"
#include "Time.h"
#include <fstream>
#include <exception>
#include <utility>
#include <assert.h>

enum OrderType { SELL, BUY };

class Bookkeeper{
private:
	class Transaction{
	public:
		// Default constructor is here so that it can be used in a list
		Transaction();
		Transaction(unsigned newSenderId, unsigned newReceiverId, 
			double newFmiCoins, time_t newTransactionTime = time(NULL));
		Transaction(const Transaction&);
		Transaction& operator=(const Transaction&);
		~Transaction();

		void copyFrom(const Transaction&);

		time_t transactionTime;
		unsigned senderId;
		unsigned receiverId;
		double fmiCoins;
	};

	class Order{
	public:
		Order();
		Order(OrderType newType, unsigned newWalletId, double newFmiCoins);
		Order(const Order&);
		Order& operator=(const Order&);
		~Order();

		void copyFrom(const Order&);

		OrderType type;
		unsigned walletId;
		double fmiCoins;
	};
public:
	Bookkeeper();
	Bookkeeper(const Bookkeeper&);
	Bookkeeper& operator=(const Bookkeeper&);
	~Bookkeeper();

	// The name is not const because it will be cut to maxNameLength if it is too long
	void addWallet(double fiatMoney, const String& name);
	void makeOrder(OrderType orderType, double fmiCoins, unsigned walletID);
	void walletInfo(unsigned walletID)const;
	void attractInvestors()const;

	// Not const because it has to modify alreadyOpenedWallets and alreadyMadeTransactions
	void saveToFile();

	// Returns the number of opened wallets
	unsigned getLastWalletID();
private:
	void copyFrom(const Bookkeeper&);

	void saveWalletsToFile()const;
	void saveOrdersToFile()const;

	void makeSellOrder(double fmiCoins, unsigned walletID);
	void makeBuyOrder(double fmiCoins, unsigned walletID);

	void saveTransactionToFile(unsigned newSenderId, unsigned newReceiverId,
		double newFmiCoins);

	// Returns the index if the ID exists and -1 if it doesn't
	int walletOwnerIndex(const String& name)const;

	void loadExistingWallets();
	void loadExistingOrders();

	double fmiCoinsUnder(unsigned walletID)const;

	void createFile(const char*)const;
	//void removeFile(const char*)const;
	bool fileExists(const char*)const;

	// Wallet IDs will be their respective index in the list of wallets in the Bookkeeper class
	Vector<std::pair<String, double>> wallets;
	// <orders.dat> has a single letter 's' or 'b' in the beginning 
	// indicating the type of orders currently open
	List<Order> orders;

	
	static const unsigned systemID;
	static const unsigned maxNameLength;
	static const double fmiCoinValue;
};

#endif