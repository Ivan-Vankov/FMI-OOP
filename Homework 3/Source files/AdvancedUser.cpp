#include "AdvancedUser.h"

AdvancedUser::AdvancedUser(int newAge, const String& newName,
	bool newBlocked) :
	BasicUser(newAge, newName, newBlocked) {}

bool AdvancedUser::updateBlockStatus(bool isBlocked, BasicUser* subject) {
	if (!subject) {
		std::cerr << "Invalid user pointer." << std::endl;
		return false;
	}
	subject->blocked = isBlocked;
	return true;
}
