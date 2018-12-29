#include "Date.h"

Date::Date(int dd, int mm, int yyyy) {
	if (verifyDate(dd, mm, yyyy)) {
		day = dd;
		month = mm;
		year = yyyy;
	}
	else {
		throw std::exception("Invalid date parameters.");
	}
}

Date::Date(const Date& other) {
	copyFrom(other);
}

Date& Date::operator=(const Date& other) {
	if (this != &other) {
		copyFrom(other);
	}
	return *this;
}

bool Date::operator>(const Date& other) const {
	if (year > other.year &&
		month > other.month &&
		day > other.day) {
		return true;
	}
	return false;
}

bool Date::operator>=(const Date& other) const {
	return !((*this) < other);
}

bool Date::operator<(const Date& other) const {
	if (year < other.year &&
		month < other.month &&
		day < other.day) {
		return true;
	}
	return false;
}

bool Date::operator<=(const Date& other) const {
	return !((*this) > other);
}

bool Date::operator==(const Date& other) const {
	if (day == other.day &&
		month == other.month &&
		year == other.year) {
		return true;
	}
	return false;
}

bool Date::operator!=(const Date& other) const {
	return !((*this) == other);
}

Date& Date::operator++() {
	if (day < 28) {
		++day;
		return *this;
	}
	if (month != 2 &&
		day < 30) {
		++day;
		return *this;
	}
	if (month == 4 || month == 6 ||
		month == 9 || month == 11) {
		if (day == 30) {
			day = 1;
			++month;
			return *this;
		}
	}
	if (month == 1 || month == 3 ||
		month == 5 || month == 7 ||
		month == 8 || month == 10 ||
		month == 12) {
		if (day == 30) {
			++day;
			return *this;
		}
		if (day == 31) {
			day = 1;
			if (month == 12) {
				++year;
				month = 1;
				return *this;
			}
			else {
				++month;
				return *this;
			}
		}
	}
	if (month == 2) {
		if (!leapYear(year)) {
			if (day == 28) {
				day = 1;
				++month;
				return *this;
			}
		}
		else {
			if (day == 28) {
				++day;
				return *this;
			}
			if (day == 29) {
				day = 1;
				++month;
				return *this;
			}
		}
	}
	return *this;
}

Date Date::operator++(int) {
	Date temp(*this);
	++(*this);
	return temp;
}

Date& Date::operator--() {
	if (day > 1) {
		--day;
		return *this;
	}
	if (day == 1) {
		if (month > 1) {
			--month;
			if (month == 1 || month == 3 ||
				month == 5 || month == 7 ||
				month == 8 || month == 10) {
				day = 31;
				return *this;
			}
			if (month == 4 || month == 6 ||
				month == 9 || month == 11) {
				day = 30;
				return *this;
			}
			if (month == 2) {
				if (!leapYear(year)) {
					day = 29;
					return *this;
				}
				else {
					day = 28;
					return *this;
				}
			}
		}
		else {
			--year;
			month = 12;
			day = 31;
			return *this;
		}
	}
	return *this;
}

Date Date::operator--(int) {
	Date temp(*this);
	--(*this);
	return temp;
}

// Number of days from other to this date
int Date::operator-(const Date& other) const {
	int days = 0;
	if ((*this) >= other) {
		return daysFromTo(other, *this);
	}
	else {
		return daysFromTo(*this, other);
	}
}

Date::~Date() {
	// No dynamic data to delete
}

void Date::copyFrom(const Date& other) {
	day = other.day;
	month = other.month;
	year = other.year;
}

int Date::daysFromTo(const Date& from, const Date& to) const {
	int days = 0;
	int nextCentury = (from.year / 100) + 1;
	days -= daysFromStartOfYearToDate(from);
	if (to.year < nextCentury * 100) {
		int yearGap = to.year - from.year;
		days += yearGap * 365;
		days += yearGap / 4;
		if ((yearGap / 4) &&
			from.year % 100 == 0 &&
			from.year % 400 != 0) {
			--days;
		}
	}
	else {
		// Days until next century
		int fromToNextcentury = nextCentury * 100 - from.year;
		days += fromToNextcentury * 365;
		days += fromToNextcentury / 4;
		if ((fromToNextcentury / 4) &&
			from.year % 100 == 0 &&
			from.year % 400 != 0) {
			--days;
		}
		int century = to.year / 100;
		// In a century where the beginning year is not a leap year
		// 365*76 + 366*24
		int daysInCentury = 36524;
		days += daysInCentury * (nextCentury - century);
		while (nextCentury < century) {
			// Check if the first year in the century is a leap year (1900, 2000...)
			if (leapYear(nextCentury++ * 100)) {
				++days;
			}
		}
		// Days until next century
		int yearsLeft = to.year - century * 100;
		days += yearsLeft * 365;
		days += yearsLeft / 4;
		if ((yearsLeft / 4) &&
			(century * 100) % 400 != 0) {
			--days;
		}
	}
	days += daysFromStartOfYearToDate(to);
	return days;
}

bool Date::verifyDate(int dd, int mm, int yyyy) {
	if (dd < 1 || dd > 31 ||
		mm < 1 || mm > 12) {
		return false;
	}
	if ((mm == 4 || mm == 6 ||
		mm == 9 || mm == 11) &&
		dd == 31) {
		return false;
	}
	if (mm == 2 && dd > 29) {
		return false;
	}
	// (not a)leap year check
	if (mm == 2 &&
		!leapYear(yyyy) &&
		dd == 29) {
		return false;
	}
	return true;
}

bool Date::leapYear(int yyyy) {
	return ((yyyy % 4 == 0 &&
		yyyy % 100 != 0) ||
		yyyy % 400 == 0);
}

int Date::daysFromStartOfYearToDate(const Date& date) const {
	int days = 0;
	int months[12] = { 31, -1, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (leapYear(date.year)) {
		months[1] = 29;
	}
	else {
		months[1] = 28;
	}
	for (int mm = 0; mm < date.month - 1; ++mm) {
		days += months[mm];
	}
	return days += date.day - 1;
}

int Date::daysUntilEndOfYearFromDate(const Date& date) const {
	if (leapYear(date.year)) {
		return 366 - daysFromStartOfYearToDate(date);
	}
	else {
		return 365 - daysFromStartOfYearToDate(date);
	}
}