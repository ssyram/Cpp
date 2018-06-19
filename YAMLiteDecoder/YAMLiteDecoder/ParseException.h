#pragma once
#ifndef PARSE_EXCEPTION_H
#define PARSE_EXCEPTION_H

#include <string>
using std::string;
using std::to_string;

string to_string(char c);

enum ParseExceptionTypes {
	NullValue, //no any value
	KeyFormat, //it's not a valid name
	Expected,
	NotExpected,
	SameKeyName,
	InvalidToken,
	InvalidTadentLevel,
	IncompetibleVectorContent,
	UnstructuredLine,
	UnexpectedBegining
};

class ParseException
{
	ParseExceptionTypes t;
	string info;
public:
	ParseException(int lineNum, int columnNum, ParseExceptionTypes type, string appendContent = "") :t(type), info("error on line ") {
		infoForming(lineNum, columnNum);
		if (type == NotExpected ||
			type == Expected ||
			type == InvalidToken ||
			type == SameKeyName)
		{
			info += appendContent;
			info += "\'>";
		}
		if (type == IncompetibleVectorContent)
			info += appendContent + "> type";
	}
	~ParseException() = default;
	ParseExceptionTypes getType() { return t; }
	string &getInfo() { return info; }

private:
	void infoForming(int lineNum, int columnNum) {
		if (columnNum)
			info += to_string(lineNum) + ", column " + to_string(columnNum) + ": ";
		else
			info += to_string(lineNum) + ": ";
		switch (t)
		{
		case NullValue:
			info += "expected corresponding value";
			break;
		case KeyFormat:
			info += "not a valid key";
			break;
		case NotExpected:
			info += "not expected content <\'";
			break;
		case Expected:
			info += "expected content <\'";
			break;
		case SameKeyName:
			info += "duplicated key name <\'";
			break;
		case InvalidToken:
			info += "Invalid token <\'";
			break;
		case InvalidTadentLevel:
			info += "Invalid tadent";
			break;
		case IncompetibleVectorContent:
			info += "Incompetible content, expected <";
			break;
		case UnstructuredLine:
			info += "not a valid line, containing too much content";
			break;
		case UnexpectedBegining:
			info += "Invalid begining";
			break;
		default:
			break;
		}
	}
};

#endif // !PARSE_EXCEPTION_H
