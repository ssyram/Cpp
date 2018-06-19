#pragma once
#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include "ParseException.h"
#include "Line.h"
#include <stack>
using std::stack;
using std::pair;

class SyntaxAnalyzer {
	stack<string> kStack; //key stack
	stack<Data> eaStack; //embed arrays stack, embed arrays need to be wrapped in Data
	Line *tLine; //temp line

public:
	SyntaxAnalyzer() {
		analyze();
	}

private:
	void analyze();
	void parseALine();

	void generateException(ParseExceptionTypes type, const string &content = "") {
		throw ParseException(tLine->lineNum(), 0, type, content);
	}

	void parseBeginWithKey();
	void parseBeginWithLineToken();

	//must make sure the top of the stack is of type DictionaryVector
	bool findSameKey(const string &keyName) {
		DictionaryVector &v = (kStack.size() ? *(DictionaryVector *)eaStack.top().getData() : keyValueTable);

		for (int i = 0; i < v.size(); ++i)
			if (v[i].first == keyName)
				return true;

		//if (!kStack.size()) {
		//	for (int i = 0; i < keyValueTable.size(); ++i)
		//		if (keyValueTable[i].first == keyName)
		//			return true;
		//}
		//else {
		//	if (kStack.top().second != KeyState::Dictionary)
		//		generateException(ParseExceptionTypes::IncompetibleVectorContent, "array");
		//	EmbedArray &a = *(EmbedArray *)eaStack.top()->getData();
		//	for (int i = 0; i < a.size(); ++i)
		//		if (((KeyValuePair *)a[i].getData())->first == keyName)
		//			return true;
		//}
		return false;

	}

	void addToStackTopArray(const Data &d) {
		if (eaStack.top().type() == DataTypes::Dictionary)
			generateException(ParseExceptionTypes::IncompetibleVectorContent, "key value pair type");
		if (eaStack.top().type() == DataTypes::UndeterminedVector)
			eaStack.top().setValue(DataTypes::Array, new ArrayVector);
		((ArrayVector *)eaStack.top().getData())->emplace_back(d);
	}

	void addToStackTopDictionary(const string &key, const Data &value) {
		if (eaStack.size()) {
			if (eaStack.top().type() == DataTypes::Array)
				generateException(ParseExceptionTypes::IncompetibleVectorContent, "value type");
			if (eaStack.top().type() == DataTypes::UndeterminedVector)
				eaStack.top().setValue(DataTypes::Dictionary, new DictionaryVector);
		}

		if (findSameKey(key))
			generateException(ParseExceptionTypes::SameKeyName);
		if (eaStack.size())
			((DictionaryVector *)eaStack.top().getData())->emplace_back(KeyValuePair(key, value));
		else
			keyValueTable.emplace_back(KeyValuePair(key, value));
	}

	int wrapArrayAndAddToTable();
	//void insertKeyValuePair(const string &keyName, const Data &value) {
	//	if (findSameKey(keyName))
	//		generateException(ParseExceptionTypes::SameKeyName);
	//	if (kStack.size())
	//		((EmbedArray *)(eaStack.top()->getData()))->emplace_back(
	//			Data(DataTypes::Pair, &KeyValuePair(keyName, value))
	//		);
	//	else
	//		keyValueTable.emplace_back(KeyValuePair(keyName, value));
	//}
	int openUpANewEmbedArray(const string &keyName = "");
	int insertLinePairToStackTopDictionary();
	int insertLineValueToStackTopArray();
};

#endif // !SYNTAX_ANALYZER_H
