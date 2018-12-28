#ifndef _FMIBOOK_HEADER_
#define _FMIBOOK_HEADER_

#include "AdvancedUser.h"
#include "IMGPost.h"
#include "URLPost.h"
#include "TXTPost.h"
#include "Vector.h"
#include <assert.h>

class FmiBook {
	friend class Admin;
	class Admin : public AdvancedUser {
	public:
		Admin(int adminAge, const String& adminName, FmiBook* newFmiBook);

		virtual bool addUser(UserType type, const String& id, int age)const;

		virtual bool removeUser(const String& subject);

		virtual ~Admin() {}
	private:
		FmiBook * fmiBook;
	};
public:
	FmiBook(int adminAge, const String& adminName);
	~FmiBook();
	bool changeUserName(const String& actor, const String& newName);
	bool nameIsTaken(const String& userName)const;

	bool addUser(const String& actor, UserType type, const String& newName, int age);
	bool removeUser(const String& actor, const String& subject);

	// Method for both blocking and unblocking users
	bool updateBlockStatus(bool isBlocked, const String& actor, const String& subject);

	bool post(const String& actor, PostType type, const String& post);
	bool removePost(const String& actor, int postNum);
	bool viewPost(int postNum)const;
	bool viewAllPosts(const String& subject)const;

	void info()const;
private:
	FmiBook(const FmiBook&) = delete;
	FmiBook& operator=(const FmiBook&) = delete;

	void addUser(BasicUser*);
	void removeUser(int index);

	int usernameIndex(const String& userName)const;

	bool getActorAndSubjectIndexes(const String& actor, const String& subject,
		int& actorIndex, int& subjectIndex);

	void resizePosts(int newSize);
	int getPostIndex(int postNum)const;

	void removeAllPostByUser(int index);

	void cleanPosts();

	Vector<BasicUser*> users;
	BasicPost** posts;
	// Number indicating the postNum of a new post
	int newPostNum;
	int postCnt;
	int postAllocSize;
};

#endif // _FMIBOOK_HEADER_