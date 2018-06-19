#pragma once
#ifndef TERMINAL_TYPE_H
#define TERMINAL_TYPE_H

#include <unordered_map>
#include <string>
#include <vector>
using std::pair;
using std::vector;
using std::string;
using std::unordered_map;

enum DataTypes
{
	DefaultValue,
	KeyString, //Can be Key or simply String: in YAMLite, can only be Key; in YAML, can combine to form String
	UndeterminedVector, //not yet to determine whether it's array or dictionary, no actual data
	Array, //data is of type vector<Data>
	Dictionary, //data is of type vector<pair<string, Data>>
	String,
	Boolean,
	Integer,
	Double,
	SciDec, //data is struct SciDecimal, containing coef and exp
	ColonToken, //no data
	LineToken, //no data
};

bool isValueType(DataTypes type);

struct SciDecimal
{
	SciDecimal(double coef, int exp) :coef(coef), exp(exp) {}
	double coef;
	int exp;
};

/*
 *Three kinds of usage:
 *	Represent all line elements, including key and value
 *	Represent Key Value Pair in the keyValueTable
 *	When it's type is Vector, the vector contains other data
 */
class Data {
	DataTypes t;
	void *v;
public:
	//can only input ColonToken or LineToken or UnderterminedVector
	Data(DataTypes tokenType) :t(tokenType), v(nullptr) {}
	Data(const string &keyName) :t(KeyString), v(new string(keyName)) {}
	Data() :t(DefaultValue), v(nullptr) {}
	Data(DataTypes type, void *value) :t(type), v(value) {}

	Data(const Data &d);
	Data(Data &&d) :t(d.t), v(d.v) {
		d.v = nullptr;
	}
	void Data::operator=(const Data &d);

	//if the value have been set, return 0, else return 1
	int setValue(DataTypes type, void *value) {
		t = type;
		v = value;
		return 1;
	}
	~Data();
	void *getData() const { return v; }
	DataTypes type() const { return t; }
	string dataToString(int ilv = 0, bool changeLine = false) const;
	Data *find(const string &key) const;
	Data *find(int index) const;
};

typedef pair<string, Data> KeyValuePair;

typedef vector<KeyValuePair> DictionaryVector;
typedef vector<Data> ArrayVector;

//total hash table
extern DictionaryVector keyValueTable;
extern Data totalData;

#endif // !TERMINAL_TYPE_H
