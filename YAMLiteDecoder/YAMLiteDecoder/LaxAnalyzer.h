#pragma once
#ifndef LAX_ANALYZER_H
#define LAX_ANALYZER_H

#include <fstream>
#include "Line.h"
#include "ParseException.h"

using std::fstream;

//take the responsibility to analyze a text field
//classify them into Data
class LexAnalyzer {
	fstream field;
	string tempLine;
	size_t lineNum;
	size_t beginIndex, toIndex;
public:
	LexAnalyzer(const string &filePath) : lineNum(1) {
		field.open(filePath);
		analyze();
		field.close();
	}
	~LexAnalyzer() {
	}

private:
	int analyze();
	int analyzeALine();

	void parseAsKey(Line &l);
	void parseAsNumber(Line &l);
	void parseAsString(Line &l);
	void parseAsColonToken(Line &l) {
		l.push(Data(DataTypes::ColonToken));
		++beginIndex;
	}
	void parseAsLineToken(Line &l) {
		l.push(Data(DataTypes::LineToken));
		++beginIndex;
	}

	bool isNumber(char c) {
		return ((c >= '0' && c <= '9') || c == '.');
	}
	bool toNumberClass() {
		char c = tempLine[beginIndex];
		if ((c >= '0' && c <= '9') || c == '.')
			return true;
		if (c == '-')
			return (beginIndex < tempLine.length() && isNumber(tempLine[beginIndex + 1]));
		return false;
	}
	bool toKey() {
		char c = tempLine[beginIndex];
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}
	bool toColonToken() { return tempLine[beginIndex] == ':'; }
	bool toLineToken() { return tempLine[beginIndex] == '-'; }
	bool toString() { return tempLine[beginIndex] == '\"'; }
	bool isMeaningless() {
		return tempLine[beginIndex] == ' ';
	}

	//if it's a meaningless token, return 0
	int classifyToken(Line &l);

	void setAsKey(Line &l, const string &s) {
		l.push(Data(s));
	}

};

#endif // !LAX_ANALYZER_H
