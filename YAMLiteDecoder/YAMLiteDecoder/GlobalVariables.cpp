#include "Line.h"
#include "TerminalType.h"

vector<Line> parsedLines;
DictionaryVector keyValueTable;
string to_string(char c) { return{ c, '\0' }; }

Data totalData(DataTypes::Dictionary, &keyValueTable);

Data::~Data() { if (v && v != &keyValueTable) delete v; }

bool isValueType(DataTypes type) {
	switch (type)
	{
	case String:
	case Boolean:
	case Integer:
	case Double:
	case SciDec:
		return true;
	default:
		break;
	}
	return false;
}
void Data::operator=(const Data &d) {
	t = d.t;
	switch (t)
	{
	case String:
	case KeyString:
		v = new string(*(string *)d.v);
		break;
	case Array:
		v = new ArrayVector(*(ArrayVector *)d.v);
		break;
	case Dictionary:
		v = new DictionaryVector(*(DictionaryVector *)d.v);
		break;
	case Boolean:
		v = new bool(*(bool *)d.v);
		break;
	case Integer:
		v = new int(*(int *)d.v);
		break;
	case Double:
		v = new double(*(double *)d.v);
		break;
	case SciDec:
		v = new SciDecimal(*(SciDecimal *)d.v);
		break;
	default:
		v = nullptr;
		break;
	}
}