#include "Line.h"
#include <iostream>
#include "MainStuff.h"
using std::to_string;
using std::cout;
using std::endl;

void Line::printLine() {
	cout << lineNumber << " ";
	for (int i = 0; i < indentLevel; ++i)
		cout << "\t";
	for (int i = 0; i < typeVector.size(); ++i) {
		Data &d = typeVector[i];
		cout << "<";
		switch (d.type())
		{
		case KeyString:
			cout << "KeyString, " << *(string *)d.getData();
			break;
		case String:
			cout << "String, " << *(string *)d.getData();
			break;
		case Boolean:
			cout << "Boolean, " << *(bool *)d.getData();
			break;
		case Integer:
			cout << "Integer, " << *(int *)d.getData();
			break;
		case Double:
			cout << "Float, " << *(double *)d.getData();
			break;
		case SciDec:
			cout << "SciDec, " << ((SciDecimal *)(d.getData()))->coef
				<< "E" << ((SciDecimal *)(d.getData()))->exp;
			break;
		case ColonToken:
			cout << "Token, \':\'";
			break;
		case LineToken:
			cout << "Token, \'-\'";
			break;
		default:
			break;
		}
		cout << "> ";
	}
	cout << ";" << endl;
}

void arrayToString(string &s, const ArrayVector &a, int ilv) {
	s += "[\n";
	++ilv;
	for (auto i = a.begin(); i != a.end(); ++i) {
		for (int k = 0; k < ilv; ++k)
			s += "\t";
		s += i->dataToString(ilv);
		s += ",\n";
	}
	s.erase(s.end() - 2, s.end() - 1);
	--ilv;
	for (int k = 0; k < ilv; ++k)
		s += "\t";
	s += "]";
}
void dictionaryToString(string &s, const DictionaryVector &d, int ilv) {
	s += "{\n";
	++ilv;
	for (auto i = d.begin(); i != d.end(); ++i) {
		for (int k = 0; k < ilv; ++k)
			s += "\t";
		s += "\"";
		s += i->first + "\"";
		s += ": ";
		s += i->second.dataToString(ilv);
		s += ",\n";
	}
	s.erase(s.end() - 2, s.end() - 1);
	--ilv;
	for (int k = 0; k < ilv; ++k)
		s += "\t";
	s += "}";
}

string Data::dataToString(int ilv, bool cl) const {
	string r = cl ? "\n" : "";
	if (cl)
		for (int i = 0; i < ilv; ++i)
			r += "\t";
	switch (t)
	{
	case KeyString:
		r += *(string *)v;
		break;
	case String:
		r += "\"" + *(string *)v + "\"";
		break;
	case Array:
		arrayToString(r, *(ArrayVector *)v, ilv);
		break;
	case Dictionary:
		dictionaryToString(r, *(DictionaryVector *)v, ilv);
		break;
	case Boolean:
		if (*(bool *)v)
			r += "true";
		else
			r += "false";
		break;
	case Integer:
		r += to_string(*(int *)v);
		break;
	case Double:
		r += to_string(*(double *)v);
		break;
	case SciDec:
		r += to_string(((SciDecimal *)v)->coef)
			+ "E"
			+ to_string(((SciDecimal *)v)->exp);
		break;
	default:
		throw string("this type can't be turn into string.");
		break;
	}
	return r;
}

Data * Data::find(const string & key) const
{
	if (t != DataTypes::Dictionary)
		throw MainStuffException("Not a valid path, what you are trying to search is not a key value pair list.");
	for (auto i = ((DictionaryVector *)v)->begin(); i != ((DictionaryVector *)v)->end(); ++i)
		if (i->first == key)
			return &i->second;
	return nullptr;
}

Data::Data(const Data &d) {
	this->operator=(d);
}