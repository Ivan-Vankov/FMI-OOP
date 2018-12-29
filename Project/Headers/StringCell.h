#ifndef _STRINGCELL_HEADER_
#define _STRINGCELL_HEADER_
#include "TableDelegate.h"
#include "Cell.h"

class StringCell : public Cell {
public:
	StringCell(const String& = "");
	StringCell(const StringCell&);
	StringCell& operator=(const StringCell&);
	virtual ~StringCell();

	int getLength() const override;
	String getContent() const override;
	String getValue() const override;
	void printCell() const override;
	Cell* clone() const override;

	static bool verifyData(const String& content);
private:
	String content;
};

#endif // _STRINGCELL_HEADER_
