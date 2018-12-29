#ifndef _DATECELL_HEADER_
#define _DATECELL_HEADER_
#include "Cell.h"
#include "Date.h"
#include "Vector.h"
#include "TableDelegate.h"

class DateCell : public Cell {
public:
	DateCell(const String&);
	DateCell(const DateCell&);
	DateCell& operator=(const DateCell&);
	virtual ~DateCell();

	int getLength() const override;
	String getContent() const override;
	String getValue() const override;
	void printCell() const override;
	Cell* clone() const override;

	static bool verifyData(const String& content);
private:
	void copyFrom(const DateCell&);

	// Gets the three numbers in some datacell content
	void getNums(const String& content, int& num1, int& num2, int& num3) const;

	// Converts 02018.012.007 to 2018.12.07
	void toRegularForm(int num1, int num2, int num3);

	// false means dd.mm.yyyy
	// true  means yyyy.mm.dd
	bool order;
	String content;
	static const Date startDate;
};

#endif // _DATECELL_HEADER_
