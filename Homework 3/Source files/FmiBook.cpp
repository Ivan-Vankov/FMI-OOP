#include "FmiBook.h"

FmiBook::Admin::Admin(int adminAge, const String& adminName, FmiBook* newFmiBook) :
	AdvancedUser(adminAge, adminName, false), fmiBook(newFmiBook) {}

bool FmiBook::Admin::addUser(UserType type, const String& id, int age)const {
	if (age < 0) {
		std::cerr << "Invalid user age." << std::endl;
		return false;
	}
	int nameInUse = fmiBook->nameIsTaken(id);
	if (nameInUse) {
		std::cerr << "A user with that name already exists." << std::endl;
		return false;
	}
	switch (type) {
	case(user):
		fmiBook->addUser(new BasicUser(age, id, false)); break;
	case(moderator):
		fmiBook->addUser(new AdvancedUser(age, id, false)); break;
	default: throw std::exception("Unknown basic user type.");
	}
	return true;
}

bool FmiBook::Admin::removeUser(const String& subject) {
	int subjectIndex = fmiBook->usernameIndex(subject);
	if (subjectIndex == -1) {
		std::cerr << "A user with that name already exists." << std::endl;
		return false;
	}
	if (!strcmp(id, subject)) {
		std::cerr << "The admin can't be removed." << std::endl;
		return false;
	}
	fmiBook->removeAllPostByUser(subjectIndex);
	// Then remove the user himself
	fmiBook->removeUser(subjectIndex);
	return true;
}

FmiBook::FmiBook(int adminAge, const String& adminName) :
	posts(nullptr), newPostNum(0), postCnt(0), postAllocSize(0) {
	if (adminAge < 0) {
		throw std::exception("Invalid admin age.");
	}
	Admin* admin = new Admin(adminAge, adminName, this);
	users.pushBack(admin);
}

FmiBook::~FmiBook() {
	cleanPosts();
}

bool FmiBook::addUser(const String& actor, UserType type, const String& newName, int age) {
	int actorIndex = usernameIndex(actor);
	if (actorIndex == -1) {
		std::cerr << "No actor with that username exists." << std::endl;
		return false;
	}
	return users[actorIndex]->addUser(type, newName, age);
}

void FmiBook::addUser(BasicUser* user) {
	users.pushBack(user);
}

bool FmiBook::removeUser(const String& actor, const String& subject) {
	int actorIndex = usernameIndex(actor);
	if (actorIndex == -1) {
		std::cerr << "No actor with that username exists." << std::endl;
		return false;
	}
	return users[actorIndex]->removeUser(subject);
}

void FmiBook::removeUser(int index) {
	users.remove(index);
}

bool FmiBook::updateBlockStatus(bool isBlocked, const String& actor, const String& subject) {
	int actorIndex, subjectIndex;
	if (!getActorAndSubjectIndexes(actor, subject, actorIndex, subjectIndex)) {
		return false;
	}
	return users[actorIndex]->updateBlockStatus(isBlocked, users[subjectIndex]);
}

bool FmiBook::post(const String& actor, PostType type, const String& post) {
	int actorIndex = usernameIndex(actor);
	if (actorIndex == -1) { return false; }

	if (users[actorIndex]->isBlocked()) {
		std::cerr << "Post not created - user blocked!" << std::endl;
		return false;
	}

	// If there is not enough space for an aditional post
	if (postCnt == postAllocSize) {
		// resize <posts> so that there is enough space
		resizePosts(postAllocSize ? 2 * postAllocSize : 2);
	}
	switch (type) {
	case(txt): posts[postCnt++] = new TXTPost(newPostNum++, users[actorIndex], post); break;
	case(url): {
		String postUrl = post.wordNumber(1);
		const char* description = post.cString() + postUrl.getLength() + 1;
		posts[postCnt++] = new URLPost(newPostNum++, users[actorIndex], postUrl, description); break; }
	case(img): posts[postCnt++] = new IMGPost(newPostNum++, users[actorIndex], post); break;
	default: throw std::exception("Unknown post type.");
	}
	return true;
}

void FmiBook::resizePosts(int newSize) {
	BasicPost** buffer = new BasicPost*[newSize];
	int lowerSize = newSize < postCnt ? newSize : postCnt;
	int i = 0, j = 0;
	// We only copy the non-null pointers to the buffer
	for (; i < lowerSize; ++i) {
		if (posts[i]) {
			buffer[j++] = posts[i];
		}
	}
	int newPostCnt = j;
	// We initialize what is left of the buffer as nullptr
	while (j < newSize) {
		buffer[j++] = nullptr;
	}
	delete[] posts;
	posts = buffer;
	postCnt = newPostCnt;
	postAllocSize = newSize;
}

bool FmiBook::removePost(const String& actor, int postNum) {
	int actorIndex = usernameIndex(actor);
	if (actorIndex == -1) { return false; }

	int postIndex = getPostIndex(postNum);
	if (postIndex == -1) { return false; }

	if (!users[actorIndex]->isAdvancedUser() && posts[postIndex]->author != users[actorIndex]) {
		std::cerr << "A basic user can't remove another person's post." << std::endl;
		return false;
	}
	delete posts[postIndex];
	posts[postIndex] = nullptr;
	return true;
}

bool FmiBook::viewPost(int postNum)const {
	int postIndex = getPostIndex(postNum);
	if (postIndex == -1) { return false; }
	posts[postIndex]->generateFile();
	return true;
}

bool FmiBook::viewAllPosts(const String& subject)const {
	int subjectIndex = usernameIndex(subject);
	if (subjectIndex == -1) { return false; }

	String fileName(subject);
	fileName + "Posts.html";
	std::ofstream ofile(fileName, std::ios::trunc);

	if (!ofile.is_open()) {
		std::cerr << "Couldn't open file." << std::endl;
		return false;
	}

	BasicPost::generateFileBeginning(ofile);
	for (int i = 0; i < postCnt; ++i) {
		if (posts[i] && posts[i]->author == users[subjectIndex]) {
			posts[i]->serializeContent(ofile);
		}
	}
	BasicPost::generateFileEnd(ofile);
	return true;
}

int FmiBook::getPostIndex(int postNum)const {
	// Posts are always in sorted order (and may have a few nullptrs in them)
	// Example postsNums: 1,2,null,null,6,8,null 
	for (int i = 0; i < postCnt; ++i) {
		if (posts[i] && posts[i]->postNum == postNum) {
			return i;
		}
	}
	std::cerr << "No post with that number exists." << std::endl;
	return -1;
}

void FmiBook::removeAllPostByUser(int index) {
	for (int i = 0; i < postCnt; ++i) {
		if (posts[i] && posts[i]->author == users[index]) {
			delete posts[i];
			posts[i] = nullptr;
		}
	}
}


void FmiBook::info()const {
	// How many basic users and mods there are
	// Which is the user with the most posts
	// How many and who are blocked in FmiBook
	// Who is the youngest and who is the oldest user

	// Counters for how many mods and basic users there are
	int basicUsersCnt = 0;
	int modsCnt = 0;
	// Vector where we count how many times a user has posted
	Vector<int> postsCnt(users.getSize(), 0);
	// Index of the biggest poster
	int biggestPoster = 0;
	// Indexes of the blocked users
	Vector<int> blockedUsers;
	// Indexes of the youngest and oldest users
	int youngestUserIndex = 0;
	int oldestUserIndex = 0;

	// Check if the admin is blocked
	if (users[0]->isBlocked()) { blockedUsers.pushBack(0); }

	// Go through all users
	for (int i = 1; i < users.getSize(); ++i) {
		// Check if the current user is basic or a mod
		if (!users[i]->isAdvancedUser()) {
			++basicUsersCnt;
		}
		else {
			++modsCnt;
		}
		// Check if the current user is blocked
		if (users[i]->isBlocked()) {
			blockedUsers.pushBack(i);
		}
		// Check if the current user is younger than the youngest up to this point
		if (users[i]->getAge() < users[youngestUserIndex]->getAge()) {
			youngestUserIndex = i;
		}
		// And check if he is the oldes
		if (users[i]->getAge() > users[oldestUserIndex]->getAge()) {
			oldestUserIndex = i;
		}
	}
	// Go through all posts
	for (int j = 0; j < postCnt; ++j) {
		// If the post is not null
		if (posts[j]) {
			int authorIndex = usernameIndex(posts[j]->author->getName());
			assert(authorIndex != -1);
			// Increment the post's author's index in postsCnt
			++postsCnt[authorIndex];
			// And check if the author is now the biggest poster
			if (postsCnt[authorIndex] > postsCnt[biggestPoster]) {
				biggestPoster = authorIndex;
			}
		}
	}

	// How many basic users and mods there are
	std::cout << "There are " << basicUsersCnt << " basic users." << std::endl;
	std::cout << "There are " << modsCnt << " moderators." << std::endl;

	// Which is the user with the most posts
	std::cout << "Most posts made by: " << users[biggestPoster]->getName()
		<< " (" << postsCnt[biggestPoster] << " post/s)." << std::endl;

	// How many and who are blocked in FmiBook
	if (!blockedUsers.getSize()) {
		std::cout << "There are no blocked users." << std::endl;
	}
	else {
		std::cout << "Blocked users:" << std::endl;
		for (int l = 0; l < blockedUsers.getSize(); ++l) {
			std::cout << users[blockedUsers[l]]->getName() << std::endl;
		}
	}

	// Who is the youngest and who is the oldest user
	std::cout << "Youngest user: " << users[youngestUserIndex]->getName() << " " << users[youngestUserIndex]->getAge() << std::endl;
	std::cout << "Oldest user: " << users[oldestUserIndex]->getName() << " " << users[oldestUserIndex]->getAge() << std::endl;
}

bool FmiBook::getActorAndSubjectIndexes(const String& actor, const String& subject,
	int& actorIndex, int& subjectIndex) {
	actorIndex = usernameIndex(actor);
	if (actorIndex == -1) { return false; }

	if (actor == subject) {
		subjectIndex = actorIndex;
	}
	else {
		subjectIndex = usernameIndex(subject);
		if (subjectIndex == -1) { return false; }
	}
	return true;
}

bool FmiBook::changeUserName(const String& actor, const String& newName) {
	int actorIndex = usernameIndex(actor);
	if (actorIndex == -1) { return false; }
	if (!strcmp(actor, newName)) {
		std::cerr << "Actor already has that name." << std::endl;
		return false;
	}
	int nameIndex = usernameIndex(newName);
	if (nameIndex == -1) {
		return users[actorIndex]->changeName(newName);
	}
	std::cerr << "Name is already taken." << std::endl;
	return false;
}

int FmiBook::usernameIndex(const String& userName)const {
	for (int i = 0; i < users.getSize(); ++i) {
		if (users[i]->getName() == userName) {
			return i;
		}
	}
	//cerr << "No user with name <" << userName << "> exists." << endl;
	return -1;
}

bool FmiBook::nameIsTaken(const String& userName)const {
	return usernameIndex(userName) != -1;
}

void FmiBook::cleanPosts() {
	for (int i = 0; i < postCnt; ++i) {
		delete posts[i];
	}
	delete[] posts;
	posts = nullptr;
}