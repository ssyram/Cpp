#include "SyntaxAnalyzer.h"

void SyntaxAnalyzer::analyze()
{
	for (int i = 0; i < parsedLines.size(); ++i) {
		tLine = &parsedLines[i];

		parseALine();
	}
	while (kStack.size())
		wrapArrayAndAddToTable();
}

void SyntaxAnalyzer::parseALine()
{
	size_t ilv = tLine->indentLv(); //current line indent level
	if (ilv > kStack.size())
		generateException(ParseExceptionTypes::InvalidTadentLevel);
	for (int i = 0; i < kStack.size() - ilv; ++i) {
		wrapArrayAndAddToTable();
	}

	//formally start tackling this line
	if (tLine->at(0).type() == DataTypes::KeyString)
		parseBeginWithKey();
	else if (tLine->at(0).type() == DataTypes::LineToken)
		parseBeginWithLineToken();
	else
		generateException(ParseExceptionTypes::UnexpectedBegining);
}

void SyntaxAnalyzer::parseBeginWithKey()
{
	if (tLine->size() < 2 || tLine->at(1).type() != DataTypes::ColonToken)
		generateException(ParseExceptionTypes::Expected, ":");

	if (tLine->size() < 3)
		openUpANewEmbedArray(*(string *)tLine->at(0).getData());
	else if (tLine->size() == 3)
		insertLinePairToStackTopDictionary();
	else
		generateException(ParseExceptionTypes::UnstructuredLine);
}

int SyntaxAnalyzer::insertLinePairToStackTopDictionary()
{
	if (!isValueType(tLine->at(2).type()))
		generateException(ParseExceptionTypes::Expected, "value type");

	addToStackTopDictionary(*(string *)tLine->at(0).getData(), tLine->at(2));

	//if (eaStack.size()) {
	//	//register
	//	KeyStatePair &kt = kStack.top();
	//	if (kt.second == KeyState::Array)
	//		generateException(ParseExceptionTypes::IncompetibleVectorContent, "Array");
	//	if (kt.second == KeyState::Undertermined)
	//		kt.second = KeyState::Dictionary;
	//	((EmbedArray *)(eaStack.top()->getData()))->emplace_back(
	//		Data(DataTypes::Pair, tLine->at(0).getData())
	//	);
	//}
	//else
	//	//insert
	//	insertKeyValuePair(*(string *)tLine->at(0).getData(), tLine->at(2));
	return 0;
}

void SyntaxAnalyzer::parseBeginWithLineToken()
{
	if (tLine->size() > 2)
		generateException(ParseExceptionTypes::UnstructuredLine);
	if (tLine->size() == 2)
		insertLineValueToStackTopArray();
	else
		openUpANewEmbedArray();
}

int SyntaxAnalyzer::insertLineValueToStackTopArray()
{
	if (!isValueType(tLine->at(1).type()))
		generateException(ParseExceptionTypes::Expected, "value type");
	if (!eaStack.size())
		generateException(ParseExceptionTypes::NotExpected, "-");

	addToStackTopArray(tLine->at(1));

	//KeyStatePair &kt = kStack.top();
	//if (kt.second == KeyState::Dictionary)
	//	generateException(ParseExceptionTypes::IncompetibleVectorContent, "Dictionary");
	//if (kt.second == KeyState::Undertermined)
	//	kt.second = KeyState::Array;

	//((EmbedArray *)(eaStack.top()->getData()))->emplace_back(tLine->at(1));

	return 0;
}

int SyntaxAnalyzer::wrapArrayAndAddToTable()
{
	Data ep = eaStack.top();
	if (ep.type() == DataTypes::UndeterminedVector)
		generateException(ParseExceptionTypes::NullValue);
	eaStack.pop();
	string kt = kStack.top();
	kStack.pop();
	if (kt == "")
		addToStackTopArray(ep);
	else
		addToStackTopDictionary(kt, ep);

	//auto kp = kStack.top();
	//if (kp.second == KeyState::Undertermined)
	//	generateException(ParseExceptionTypes::NullValue);
	//kStack.pop();
	//auto ea = eaStack.top();
	//eaStack.pop();
	//if (kp.first == "")
	//	return 1;
	//insertKeyValuePair(kp.first, *ea);
	return 0;
}

int SyntaxAnalyzer::openUpANewEmbedArray(const string &keyName)
{
	if (!kStack.size() && keyName == "")
		generateException(ParseExceptionTypes::NotExpected, "-");

	kStack.emplace(keyName);
	eaStack.emplace(Data(DataTypes::UndeterminedVector));

	//kStack.emplace(KeyStatePair(keyName, KeyState::Undertermined));
	//Data *pd = new Data(DataTypes::Vector, new EmbedArray);
	//if (eaStack.size())
	//	((EmbedArray *)(eaStack.top()->getData()))->emplace_back(*pd);
	//eaStack.emplace(pd);
	return 0;
}