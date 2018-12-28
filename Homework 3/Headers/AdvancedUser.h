#ifndef _ADVANCED_USER_
#define _ADVANCED_USER_
#include "BasicUser.h"

class AdvancedUser : public BasicUser {
public:
	AdvancedUser(int newAge, const String& newName, bool newBlocked = false);

	virtual bool isAdvancedUser()const { return true; }

	virtual bool updateBlockStatus(bool isBlocked, BasicUser* subject);

	virtual ~AdvancedUser() {}
};

#endif // _ADVANCED_USER_