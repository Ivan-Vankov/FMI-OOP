#include "TextEditor.h"

TextEditor::TextEditor(const String & fileName) {
	try {
		loadFile(fileName);
	}
	catch (std::exception&) {
		throw;
	}
}

TextEditor::~TextEditor() {
}

bool TextEditor::makeHeading(int line) {
	// Check of parameter is valid
	if (!lineCheck(line)) {
		return false;
	}
	// Check if the line has a maximum heading
	if (file[line].first.headingSize >= MAX_HEADING_SIZE) {
		std::cerr << "Line already has a maximum heading." << std::endl;
		return false;
	}
	file[line].first.headingSize++;
	return true;
}

bool TextEditor::formatText(int line, int from, int to, FormatType format) {
	// Check of parameters are valid
	if (!from_toCheck(line, from, to)) {
		return false;
	}

	// If the format is italic or combine
	// we update the italic metadata of the line
	bool isFormatedInItalic = false;
	bool isFormatedInBold = false;
	if (format == italic || format == combine) {
		isFormatedInItalic = !updateMetadata(file[line].first.italicFromTo, from, to, format);
		if (format == italic) {
			if (isFormatedInItalic) {
				std::cerr << "Line has already been formated in italic." << std::endl;
				return false;
			}
			else { return true; }
		}
	}
	// If the format is bold or combine
	// we update the bold metadata of the line
	if (format == bold || format == combine) {
		isFormatedInBold = !updateMetadata(file[line].first.boldFromTo, from, to, format);
		if (format == bold) {
			if (isFormatedInBold) {
				std::cerr << "Line has already been formated in bold." << std::endl;
				return false;
			}
			else { return true; }
		}
	}
	// If we reach here then the format is definately combine
	if (isFormatedInItalic && isFormatedInBold) {
		std::cerr << "Line has already been formated in combine." << std::endl;
		return false;
	}
	// Here we return true just so that the compiler doesn't give warnings
	return true;
}

bool TextEditor::updateMetadata(Vector<std::pair<int, int>>& metadata, int from, int to, FormatType format) {

	// cwfp is short for <currently wanted format pair> (or the pair of indexes <from> and <to>)

	for (int i = 0; i < metadata.getSize();) {
		// Check if the current pair does not intersect cwfp at all
		if (metadata[i].second < from || metadata[i].first > to) {
			// We only increment i if the current pair does not intersect cwfp
			++i;
			continue;
		}
		// If the cwfp is entirely covered by the current pair
		if (metadata[i].first <= from && metadata[i].second >= to) {
			// then we do nothing
			return false;
		}
		// If the current pair is entirely inside the cwfp 
		if (metadata[i].first > from && metadata[i].second < to) {
			// Remove it from the metadata
			metadata.remove(i);
			continue;
		}
		// If the current pair clips cwfp at the start
		if (metadata[i].first <= from && metadata[i].second >= from) {
			// We merge the two intervals, remove the pair and continue
			from = metadata[i].first;
			metadata.remove(i);
			continue;
		}
		// If the current pair clips cwfp at the start
		if (metadata[i].first <= to && metadata[i].second >= to) {
			// We merge the two intervals, remove the pair and continue
			to = metadata[i].second;
			metadata.remove(i);
			continue;
		}
	}
	// Now we just add cwfp to the metadata
	metadata.pushBack(std::make_pair(from, to));
	return true;
}

bool TextEditor::addLine(const String& lineContent) {
	file.pushBack(std::make_pair(Metadata(), lineContent));
	return true;
}

bool TextEditor::removeLine(int line) {
	// Check of parameter is valid
	if (!lineCheck(line)) {
		return false;
	}
	file.removeWithNoShuffle(line);
	return true;
}

void TextEditor::sortMetadata(FormatType format, int line) {
	if (format == italic) {
		sortMetadata(file[line].first.italicFromTo, line);
		return;
	}
	if (format == bold) {
		sortMetadata(file[line].first.boldFromTo, line);
		return;
	}
	assert(format == combine);
}

void TextEditor::sortMetadata(Vector<std::pair<int, int>>& from_toVector, int line) {
	// Maximum number of buckets
	int wordCnt = file[line].second.countWords();

	/// Normaly we would need Vector<Vector<pair<int, int>>> but there can't be more than one 
	/// pair that starts at a given index so just Vector<pair<int, int>> is enough

	// Make wordCnt as many buckets and initialize them with <-1,-1>
	Vector<std::pair<int, int>> buckets(wordCnt, std::make_pair(-1, -1));
	// Fill the buckets
	for (int i = 0; i < from_toVector.getSize(); ++i) {
		buckets[from_toVector[i].first] = from_toVector[i];
	}
	// Now refill the from_toVector
	for (int j = 0, k = 0; j < wordCnt; ++j) {
		if (buckets[j].first != -1) {
			from_toVector[k++] = buckets[j];
		}
	}
}

void TextEditor::saveAsMarkdown() {
	String mdFileName = changeExtentionTo_md();
	std::ofstream ofile(mdFileName);
	if (!ofile.is_open()) {
		std::cerr << "Couldn't open file for writing." << std::endl;
		return;
	}
	int i = 0;
	int fileSize = file.getSize();
	for (; i < fileSize; ++i) {
		// First make the appropriate heading
		int j = 0;
		for (; j < file[i].first.headingSize; ++j) {
			ofile << '#';
			if (!ofile.good()) {
				std::cerr << "Couldn't write to file." << std::endl;
				return;
			}
		}
		if (j) { ofile << ' '; }

		// The mothod is not const because here we sort the metadata
		sortMetadata(file[i].first.italicFromTo, i);
		sortMetadata(file[i].first.boldFromTo, i);

		// Indexes of the current pair in metadata
		int italicIndex = 0;
		int boldIndex = 0;
		// The numbers of pairs of italic and bold formats
		int italicPairsNum = file[i].first.italicFromTo.getSize();
		int boldPairsNum = file[i].first.boldFromTo.getSize();
		int wordCounter = 0;
		int lineLength = file[i].second.getLength();
		String delims(" \t");
		// Now we write current line to the file while keeping the formating in mind
		int k = 0;
		while (k < lineLength) {
			// If we are on a delimiter then we just skip ahead
			if (delims.isIn(file[i].second[k])) {
				ofile << file[i].second[k++];
				if (!ofile.good()) {
					std::cerr << "Couldn't write to file." << std::endl;
					return;
				}
			}
			// Otherwise we have reached a word
			else {
				// Check if the current word is formated as italic
				if (italicIndex < italicPairsNum && file[i].first.italicFromTo[italicIndex].first == wordCounter) {
					ofile << ITALIC;
				}
				// Check if the current word is formated as bold
				if (boldIndex < boldPairsNum && file[i].first.boldFromTo[boldIndex].first == wordCounter) {
					ofile << BOLD;
				}
				if (!ofile.good()) {
					std::cerr << "Couldn't write to file." << std::endl;
					return;
				}
				// Write the rest of the word
				while (k < lineLength && !delims.isIn(file[i].second[k])) {
					ofile << file[i].second[k++];
				}
				// Check if the current word is the last of a BOLD chain
				if (boldIndex < boldPairsNum && file[i].first.boldFromTo[boldIndex].second == wordCounter) {
					++boldIndex;
					ofile << BOLD;
				}
				// Check if the current word is the last of an ITALIC chain
				if (italicIndex < italicPairsNum && file[i].first.italicFromTo[italicIndex].second == wordCounter) {
					++italicIndex;
					ofile << ITALIC;
				}
				// Finally increment the wordcounter
				++wordCounter;
				if (!ofile.good()) {
					std::cerr << "Couldn't write to file." << std::endl;
					return;
				}
			}
		}
		if (i + 1 < fileSize) {
			ofile << '\n';
		}
	}
	ofile.close();
}

void TextEditor::loadFile(const String& fileName) {
	if (!validFileName(fileName)) {
		throw std::exception("File name is not valid.");
	}
	this->fileName = fileName;

	std::ifstream ifile(fileName);

	// First we determine how many lines there are in the file
	int lineCnt = 1; // 1 because we consider an empty line to still be a line
	char c;
	while (ifile.get(c)) {
		if (c == '\n') {
			++lineCnt;
		}
	}
	// Check ifile integrity again
	if (!ifstreamCheck(ifile)) {
		throw std::exception("Couldn't read from file.");
	}
	// Reset ifile
	resetIfstream(ifile);
	// Allocate lineCnt as many strings
	file.resize(lineCnt);

	// Now we count the size of each line
	int lineLength = 0;
	// We use lineCnt as an index for the lines in <file>
	lineCnt = 0;
	while (ifile.get(c)) {
		// ++lineLength first so that we have space for the '\0'
		++lineLength;
		if (c == '\n') {
			// This is so that file has an appropriate size at the end of the cycle
			file.pushBack(std::make_pair(Metadata(), ""));
			file[lineCnt++].second.resize(lineLength);
			lineLength = 0;
		}
	}
	// Outside of the lower if so that the empty line can still be read
	file.pushBack(std::make_pair(Metadata(), ""));
	// The last line will have no '\n'
	if (lineLength) {
		file[lineCnt].second.resize(lineLength);
	}
	// Check ifile integrity again
	if (!ifstreamCheck(ifile)) {
		throw std::exception("Couldn't read from file.");
	}
	// Reset ifile
	resetIfstream(ifile);

	// Now we fill <file> with the contents of the text file
	for (int i = 0; i < file.getAllocatedSize(); ++i) {
		int j = 0;
		for (; j < file[i].second.getAllocatedSize() - 1; ++j) {
			ifile.get(c);
			if (!ifstreamCheck(ifile)) {
				throw std::exception("Couldn't read from file.");
			}
			file[i].second.pushBack(c);
		}
		// For the '\n'
		ifile.get(c);
	}
	ifile.close();
}

// false if the bad or fail flags are up
// true if the good or eof flags are up
bool TextEditor::ifstreamCheck(const std::ifstream& ifile)const {
	if (!ifile.good()) {
		if (ifile.eof()) {
			return true;
		}
		return false;
	}
	return true;
}

void TextEditor::resetIfstream(std::ifstream& ifile)const {
	// Clear ifiles flags
	ifile.clear();
	// And make it point to the beginning of the file
	ifile.seekg(0, std::ios::beg);
}

bool TextEditor::validFileName(const String & name) const {
	std::ofstream ofile(name, std::ios::app);
	if (!ofile.is_open()) {
		return false;
	}
	ofile.close();
	return true;
}

bool TextEditor::lineCheck(int line) const {
	if (line < 0 || line >= file.getSize()) {
		std::cerr << "Invalid <line> index." << std::endl;
		return false;
	}
	return true;
}

bool TextEditor::from_toCheck(int line, int from, int to)const {
	if (!lineCheck(line)) {
		return false;
	}
	int wordCount = file[line].second.countWords();
	if (from < 0 || from >= wordCount) {
		std::cerr << "Invalid <from> index." << std::endl;
		return false;
	}
	if (to < 0 || to >= wordCount) {
		std::cerr << "Invalid <to> index." << std::endl;
		return false;
	}
	if (to < from) {
		std::cerr << "The <to> index has to be after the <from> index." << std::endl;
		return false;
	}
	return true;
}

String TextEditor::changeExtentionTo_md()const {
	// Short for extention
	int ext = fileName.getLength() - 1;
	while (fileName[ext] != '.') {
		--ext;
	}
	// Allocate enough for the string + "md" + '\0'
	String buffer(ext + 3);
	int i = 0;
	for (; i <= ext; ++i) {
		buffer.pushBack(fileName[i]);
	}
	buffer + "md";
	return buffer;
}