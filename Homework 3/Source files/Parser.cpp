#include "Parser.h"

bool Parser::parseAndExecute(const String& input, FmiBook& fmiBook) {
	int wordCount = input.countWords();
	// Do nothing if there are no words in the command
	if (!wordCount) {
		return true;
	}
	// Get the first word of the input
	String wordOne = input.wordNumber(1);

	String wordTwo = input.wordNumber(2);
	// Search if that word matches any of the know commands
	if (!strcmp(wordTwo, "add_user")) {
		parseAddUser(wordOne, input, user, wordCount, fmiBook);
		return true;
	}
	if (!strcmp(wordTwo, "add_moderator")) {
		parseAddUser(wordOne, input, moderator, wordCount, fmiBook);
		return true;
	}
	if (!strcmp(wordTwo, "remove_user")) {
		parseRemoveUser(wordOne, input, wordCount, fmiBook);
		return true;
	}
	if (!strcmp(wordTwo, "block")) {
		parseBlocking(true, wordOne, input, wordCount, fmiBook);
		return true;
	}
	if (!strcmp(wordTwo, "unblock")) {
		parseBlocking(false, wordOne, input, wordCount, fmiBook);
		return true;
	}
	if (!strcmp(wordTwo, "post")) {
		parsePost(wordOne, input, wordCount, fmiBook);
		return true;
	}
	if (!strcmp(wordTwo, "remove_post")) {
		parseRemovePost(wordOne, input, wordCount, fmiBook);
		return true;
	}
	if (!strcmp(wordTwo, "view_post")) {
		parseViewPost(input, wordCount, fmiBook);
		return true;
	}
	if (!strcmp(wordTwo, "view_all_posts")) {
		parseViewAllPost(input, wordCount, fmiBook);
		return true;
	}
	if (!strcmp(wordTwo, "rename")) {
		parseRename(wordOne, input, wordCount, fmiBook);
		return true;
	}
	if (!strcmp(wordOne, "info")) {
		if (wordCount == 1) { fmiBook.info(); }
		return true;
	}
	if (!strcmp(wordOne, "quit")) {
		if (wordCount == 1) { return false; }
	}
	std::cerr << "Unknown command." << std::endl;
	return true;
}

bool Parser::wordAmmountCheck(int wordCount, int wordAmmount) {
	if (wordCount < wordAmmount) {
		std::cerr << "Not enough words in command." << std::endl;
		return false;
	}
	if (wordCount > wordAmmount) {
		std::cerr << "Too many words in command." << std::endl;
		return false;
	}
	return true;
}

bool Parser::parseAddUser(const String& actor, const String& input, UserType type, int wordCount, FmiBook& fmiBook) {
	// Admin add_moderator Kiril 23
	// Admin add_user Vasil 30

	// Check if there number of words in the input is not valid
	if (!wordAmmountCheck(wordCount, 4)) { return false; }

	String subject = input.wordNumber(3);
	String age = input.wordNumber(4);
	if (!isUnsigned(age)) {
		std::cerr << "Fourth word should be an unsigned integer." << std::endl;
		return false;
	}
	if (fmiBook.addUser(actor, type, subject, atoi(age))) {
		std::cout << "Successfully added user." << std::endl;
		return true;
	}
	return false;
}

bool Parser::parseRemoveUser(const String& actor, const String& input, int wordCount, FmiBook& fmiBook) {
	// Admin remove_user Vasil
	if (!wordAmmountCheck(wordCount, 3)) { return false; }

	String subject = input.wordNumber(3);
	if (fmiBook.removeUser(actor, subject)) {
		std::cout << "Successfully removed user." << std::endl;
		return true;
	}
	return false;
}

// true for block, false for unblock
bool Parser::parseBlocking(bool blockOrUnblock, const String& actor, const String& input, int wordCount, FmiBook& fmiBook) {
	// Admin block Kiril
	// Admin unblock Kiril
	if (!wordAmmountCheck(wordCount, 3)) { return false; }

	String subject = input.wordNumber(3);
	if (fmiBook.updateBlockStatus(blockOrUnblock, actor, subject)) {
		if (blockOrUnblock) {
			std::cout << "Successfully blocked user <" << subject << ">." << std::endl;
		}
		else {
			std::cout << "Successfully unblocked user <" << subject << ">." << std::endl;
		}
		return true;
	}
	return false;
}

bool Parser::parsePost(const String& actor, const String& input, int wordCount, FmiBook& fmiBook) {
	// Kiril post[image] / home / kiril / img / hello.png
	// Admin post[url] http://www.example.com някакво описание
	// Kiril post[text] Hello, world!

	if (wordCount < 4) {
		std::cerr << "Too few words in command." << std::endl;
		return false;
	}

	String postType = input.wordNumber(3);
	PostType type;
	if (!toPostType(postType, type)) { return false; }
	if (type == url && wordCount == 4) {
		std::cerr << "Too few words in command." << std::endl;
		return false;
	}
	String postContent;
	String currToken = input.wordNumber(4);
	int i = 5;
	while (currToken.getLength()) {
		postContent + " " + currToken;
		currToken = input.wordNumber(i++);
	}
	if (fmiBook.post(actor, type, postContent)) {
		std::cout << "Successfully added post." << std::endl;
		return true;
	}
	return false;
}

bool Parser::parseRemovePost(const String& actor, const String& input, int wordCount, FmiBook& fmiBook) {
	// Kiril remove_post 8
	if (!wordAmmountCheck(wordCount, 3)) { return false; }

	String postNum = input.wordNumber(3);
	if (!isUnsigned(postNum)) {
		std::cerr << "Third word should be an unsigned integer." << std::endl;
		return false;
	}
	if (fmiBook.removePost(actor, atoi(postNum))) {
		std::cout << "Successfully removed post." << std::endl;
		return true;
	}
	return false;
}

bool Parser::parseViewPost(const String& input, int wordCount, FmiBook& fmiBook) {
	// Kiril view_post 8
	if (!wordAmmountCheck(wordCount, 3)) { return false; }

	String postNum = input.wordNumber(3);
	if (!isUnsigned(postNum)) {
		std::cerr << "Third word should be an unsigned integer." << std::endl;
		return false;
	}
	if (fmiBook.viewPost(atoi(postNum))) {
		std::cout << "Successfully created post file." << std::endl;
		return true;
	}
	return false;
}

bool Parser::parseViewAllPost(const String& input, int wordCount, FmiBook& fmiBook) {
	// Ivan view_all_posts Kiril
	if (!wordAmmountCheck(wordCount, 3)) { return false; }

	String subject = input.wordNumber(3);
	if (fmiBook.viewAllPosts(subject)) {
		std::cout << "Successfully created a file for all posts of user <" << subject << ">." << std::endl;
		return true;
	}
	return false;
}

bool Parser::parseRename(const String& actor, const String& input, int wordCount, FmiBook& fmiBook) {
	// Admin rename Pesho
	if (!wordAmmountCheck(wordCount, 3)) { return false; }

	String newName = input.wordNumber(3);
	if (fmiBook.changeUserName(actor, newName)) {
		std::cout << "Successfully changed name to <" << newName << ">." << std::endl;
		return true;
	}
	return false;
}

bool Parser::toPostType(const String& postType, PostType& type) {
	if (!strcmp(postType, "TXT")) {
		type = txt;
		return true;
	}
	if (!strcmp(postType, "URL")) {
		type = url;
		return true;
	}
	if (!strcmp(postType, "IMG")) {
		type = img;
		return true;
	}
	std::cerr << "Unknown post type." << std::endl;
	return false;
}

bool Parser::isUnsigned(const String& str) {
	if (str[0] == '-') {
		return false;
	}
	int length = str.getLength();
	if ((str[0] == '+' && length == 1) || (str[0]<'0' || str[0]>'9')) {
		return false;
	}
	for (int i = 1; i < length; ++i) {
		if (str[0]<'0' || str[0]>'9') {
			return false;
		}
	}
	return true;
}