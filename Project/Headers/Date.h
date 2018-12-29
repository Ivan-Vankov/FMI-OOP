#ifndef _DATE_HEADER_
#define _DATE_HEADER_
#include <exception>

class Date {
public:
	Date(int dd, int mm, int yyyy);
	Date(const Date&);
	Date& operator=(const Date&);
	~Date();

	bool operator>(const Date&) const;
	bool operator>=(const Date&) const;
	bool operator<(const Date&) const;
	bool operator<=(const Date&) const;
	bool operator==(const Date&) const;
	bool operator!=(const Date&) const;

	Date& operator++();
	Date operator++(int);
	Date& operator--();
	Date operator--(int);

	// Number of days from other to this date
	int operator-(const Date& other) const;

	static bool verifyDate(int dd, int mm, int yyyy);
	static bool leapYear(int yyyy);

	int daysFromStartOfYearToDate(const Date&) const;
	int daysUntilEndOfYearFromDate(const Date&) const;
private:
	void copyFrom(const Date&);

	int daysFromTo(const Date& from, const Date& to) const;

	int day;
	int month;
	int year;
};

#endif // _DATE_HEADER_