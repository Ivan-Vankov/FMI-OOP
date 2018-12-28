#ifndef _TEXTEDITOR_HEADER_
#define _TEXTEDITOR_HEADER_
#include "Vector.h"
#include "String.h"
#include <fstream>
#include <utility>
#include <assert.h>

// Default text editing types
#define ITALIC "_"
#define BOLD "**"
#define MAX_HEADING_SIZE 6

enum FormatType {
	italic,
	bold,
	combine
};

class TextEditor {
	class Metadata {
	public:
		Metadata(int newHeadingSize = 0) : headingSize(newHeadingSize) {}
		Vector<std::pair<int, int>> italicFromTo;
		Vector<std::pair<int, int>> boldFromTo;
		int headingSize;
	};
public:
	TextEditor(const String& fileName);
	~TextEditor();

	bool formatText(int line, int from, int to, FormatType format);
	bool addLine(const String& lineContent);
	bool removeLine(int line);
	bool makeHeading(int line);

	void saveAsMarkdown();

private:

	void loadFile(const String& fileName);

	bool validFileName(const String& name) const;
	bool lineCheck(int line)const;
	bool from_toCheck(int line, int from, int to) const;

	void sortMetadata(FormatType, int line);
	// Bucketsort
	void sortMetadata(Vector<std::pair<int, int>>&, int line);
	bool updateMetadata(Vector<std::pair<int, int>>& metadata, int from, int to, FormatType format);

	String changeExtentionTo_md() const;

	// false if the bad or fail flags are up
	// true if the good or eof flags are up
	bool ifstreamCheck(const std::ifstream&) const;
	void resetIfstream(std::ifstream&) const;

	Vector<std::pair<Metadata, String>> file;
	String fileName;
};

#endif