#include "Tables.h"

Tables::Tables() :tables() {}

Tables::Tables(const Tables& other) : tables(other.tables) {}

Tables& Tables::operator=(const Tables& other) {
	if (this != &other) {
		tables = other.tables;
	}
	return *this;
}

Tables::~Tables() {
	// No dynamic data to delete
}

const std::string& Tables::firstFilename() const {
	if (!tables.size()) {
		throw std::exception("No tables exist.");
	}
	return tables.begin()->first;
}

int Tables::size() const {
	return tables.size();
}

void Tables::setCell(const std::string& path, const String& RandC, const String& content) {
	tables.find(path)->second.setCell(RandC, content);
}

void Tables::addTable(const std::string& path) {
	tables.insert(std::make_pair(path, (Table)path));
}

void Tables::removeTable(const std::string& path) {
	if (!tables.size()) {
		throw std::exception("No tables to remove.");
	}
	tables.erase(path);
}

const Table& Tables::findTable(const std::string& path) const {
	tableIsOpenedCheck(path);
	return tables.find(path)->second;
}

void Tables::printTable(const std::string& path) const {
	tableIsOpenedCheck(path);
	return tables.find(path)->second.print();
}

void Tables::saveTableToFile(const std::string& path, const String& saveTo) const {
	tableIsOpenedCheck(path);
	tables.find(path)->second.saveToFile(saveTo);
}

bool Tables::tableIsOpened(const std::string& path) const {
	if (tables.find(path) == tables.cend()) {
		return false;
	}
	return true;
}

void Tables::tableIsOpenedCheck(const std::string& path) const {
	if (!tableIsOpened(path)) {
		throw std::exception("No such table exists.");
	}
}