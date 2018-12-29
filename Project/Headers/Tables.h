#ifndef _TABLES_HEADER_
#define _TABLES_HEADER_
#include "Table.h"
#include <unordered_map>

class Tables {
public:
	Tables();
	Tables(const Tables&);
	Tables& operator=(const Tables&);
	~Tables();

	//const pair<string, Table>& first() const;
	const std::string& firstFilename() const;
	int size() const;
	void setCell(const std::string& path, const String& RandC, const String& content);
	void addTable(const std::string& path);
	void removeTable(const std::string& path);
	const Table& findTable(const std::string& path) const;
	void printTable(const std::string& path) const;
	void saveTableToFile(const std::string& path, const String& saveTo) const;
	bool tableIsOpened(const std::string& path) const;

private:
	void tableIsOpenedCheck(const std::string& path) const;

	std::unordered_map<std::string, Table> tables;
};

#endif // _TABLES_HEADER_