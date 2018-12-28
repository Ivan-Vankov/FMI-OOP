#ifndef _BASIC_USER_
#define _BASIC_USER_
#include "String.h"

enum UserType {
	user,
	moderator
};

class BasicUser {
	friend class AdvancedUser;
public:
	BasicUser(int newAge, const String& newName, bool newBlocked = false);
	virtual ~BasicUser() {}

	bool isBlocked()const;
	int getAge()const;
	const String& getName()const;

	bool changeName(const String& userName);

	virtual bool isAdvancedUser()const;

	virtual bool addUser(UserType type, const String& id, int age)const;
	virtual bool removeUser(const String& id);

	////// Mothod for both blocking and unblocking users
	virtual bool updateBlockStatus(bool isBlocked, BasicUser*);
protected:
	bool blocked;
	int age;
	String id;
};

#endif // _BASIC_USER_