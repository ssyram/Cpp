#include "LaxAnalyzer.h"
using std::getline;

#include <math.h>

int LexAnalyzer::analyze()
{
	while (getline(field, tempLine)) {
		analyzeALine();
		++lineNum;
	}
	return 0;
}

int LexAnalyzer::analyzeALine()
{
	beginIndex = 0;
	for (; beginIndex < tempLine.length() && tempLine[beginIndex] == ' '; ++beginIndex);
	if (beginIndex == tempLine.length() || tempLine[beginIndex] == '#')
		return 1;
	if (beginIndex % 2)
		throw ParseException(lineNum, beginIndex, ParseExceptionTypes::Expected, " ");
	Line l(lineNum, beginIndex / 2);

	while (beginIndex < tempLine.length())
	{
		if (beginIndex + 3 < tempLine.length() && tempLine.substr(beginIndex, 4) == "true") {
			l.push(Data(DataTypes::Boolean, new bool(true)));
			beginIndex += 4;
		}
		else if (beginIndex + 4 < tempLine.length() && tempLine.substr(beginIndex, 5) == "false") {
			l.push(Data(DataTypes::Boolean, new bool(false)));
			beginIndex += 5;
		}
		else if (!classifyToken(l)) break; //if classify token return 0, it represent the show up of '#'
	}

	parsedLines.emplace_back(l);
	return 1;
}

inline bool validKeyContent(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || (c >= '0' && c <= '9');
}
void LexAnalyzer::parseAsKey(Line & l)
{
	toIndex = beginIndex;
	while (toIndex < tempLine.length() && validKeyContent(tempLine[++toIndex]));
	if (tempLine[toIndex - 1] == '_')
		throw ParseException(lineNum, toIndex, ParseExceptionTypes::KeyFormat);
	setAsKey(l, tempLine.substr(beginIndex, toIndex - beginIndex));

	beginIndex = toIndex;
}

void LexAnalyzer::parseAsNumber(Line & l)
{
	bool isSciDec = false;
	int decimal = 0;
	int iResult = 0;
	double dResult = 0;
	bool isMinus = (tempLine[beginIndex] == '-');
	bool rightIsMinus = false;
	if (isMinus)
		++beginIndex;
	while (true) {
		char c = tempLine[beginIndex++];
		if (c >= '0' && c <= '9')
			if (decimal)
				dResult += (c - '0')*pow(10, decimal--);
			else
				iResult = iResult * 10 + (c - '0');
		else if (c == '.') {
			if (decimal || isSciDec)
				throw ParseException(lineNum, beginIndex, ParseExceptionTypes::NotExpected, ".");
			dResult = iResult;
			--decimal;
		}
		else if (c == 'e' || c == 'E') {
			if (isSciDec)
				throw ParseException(lineNum, beginIndex, ParseExceptionTypes::NotExpected, to_string(tempLine[beginIndex]));
			isSciDec = true;
			if (!decimal)
				dResult = iResult;
			iResult = 0;
			decimal = 0;
		}
		else if (c == '-')
			if (tempLine[beginIndex - 1] == 'e' || tempLine[beginIndex - 1] == 'E')
				rightIsMinus = true;
			else
				throw ParseException(lineNum, beginIndex, ParseExceptionTypes::NotExpected, "-");
		else {
			--beginIndex;
			break;
		}
		if (beginIndex >= tempLine.length())
			break;
	}
	if (isSciDec) {
		if (isMinus)
			dResult = -dResult;
		if (rightIsMinus)
			iResult = -iResult;
		l.push(Data(DataTypes::SciDec, new SciDecimal(dResult, iResult)));
	}
	else if (decimal) {
		if (isMinus)
			dResult = -dResult;
		l.push(Data(DataTypes::Double, new double(dResult)));
	}
	else {
		if (isMinus)
			iResult = -iResult;
		l.push(Data(DataTypes::Integer, new int(iResult)));
	}
}

void LexAnalyzer::parseAsString(Line & l)
{
	++beginIndex;
	toIndex = beginIndex;
	for (; toIndex < tempLine.length() &&
		!(tempLine[toIndex] == '\"' && tempLine[toIndex - 1] != '\\');
		++toIndex);
	if (toIndex == tempLine.length())
		throw ParseException(lineNum, toIndex, ParseExceptionTypes::Expected, "\"");
	l.push(Data(DataTypes::String, new string(tempLine.substr(beginIndex, toIndex - beginIndex))));

	beginIndex = toIndex + 1;
}

int LexAnalyzer::classifyToken(Line & l)
{
	if (isMeaningless())
		++beginIndex;
	else if (toKey()) {
		parseAsKey(l);
		return 1;
	}
	else if (toString()) {
		parseAsString(l);
		return 5;
	}
	else if (toNumberClass()) {
		parseAsNumber(l);
		return 2;
	}
	else if (toColonToken()) {
		parseAsColonToken(l);
		return 3;
	}
	else if (toLineToken()) {
		parseAsLineToken(l);
		return 4;
	}
	else if (tempLine[beginIndex] == '#')
		return 0;
	else
		throw ParseException(lineNum, beginIndex, ParseExceptionTypes::NotExpected, to_string(tempLine[beginIndex]));
	return 1;
}
