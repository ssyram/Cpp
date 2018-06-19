#pragma once
#ifndef LINE_H
#define LINE_H

#include "TerminalType.h"

class Line {
	size_t lineNumber;
	size_t indentLevel;
	vector<Data> typeVector;
public:
	Line(size_t lineNum, size_t indentLv):lineNumber(lineNum), indentLevel(indentLv) {}

	size_t lineNum() { return lineNumber; }
	size_t indentLv() { return indentLevel; }
	Data &operator[](size_t index) { return typeVector[index]; }
	Data &at(size_t index) { return typeVector[index]; }
	void push(const Data &newOne) { typeVector.emplace_back(newOne); }
	size_t size() { return typeVector.size(); }

	void printLine();
};

extern vector<Line> parsedLines;

#endif // !LINE_H
