#include "BasicUser.h"

BasicUser::BasicUser(int newAge, const String& newName, bool newBlocked) :
	blocked(newBlocked), age(newAge), id(newName) {}

bool BasicUser::isBlocked() const { return blocked; }

int BasicUser::getAge() const { return age; }

const String& BasicUser::getName()const { return id; }

bool BasicUser::changeName(const String& userName) {
	if (userName == id) {
		std::cerr << "User already has that name name." << std::endl;
		return false;
	}
	id = userName;
	return true;
}

bool BasicUser::isAdvancedUser()const { return false; }

bool BasicUser::addUser(UserType type, const String& id, int age)const {
	std::cerr << "User <" << id << "> does not have permission to add other users.";
	return false;
}

bool BasicUser::removeUser(const String& id) {
	std::cerr << "User <" << id << "> does not have permission to remove other users.";
	return false;
}

////// Mothod for both blocking and unblocking users
bool BasicUser::updateBlockStatus(bool isBlocked, BasicUser*) {
	std::cerr << "User <" << id
		<< "> doesn't have the administrative rights to block/unblock other users." << std::endl;
	return false;
}