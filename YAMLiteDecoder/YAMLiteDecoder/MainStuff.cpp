#include "MainStuff.h"
#include "Line.h"
#include <fstream>
using std::ofstream;
using std::to_string;

int turnToInt(const char *str, int length) {
	int r = 0;
	for (int i = 0; i < length; ++i)
		if (str[i] >= '0' && str[i] <= '9')
			r = r * 10 + (str[i] - '0');
		else
			throw MainStuffException("array index not valid");
}

bool isValidFile(const string &fileName) {
	if (fileName.size() < 5 || fileName.substr(fileName.size() - 4, 4) != ".yml")
		return false;
	return true;
}

int toInt(const string &s, int b, int e) {
	int r = 0;
	for (int i = b; i < e; ++i)
		if (s[i] >= '0' && s[i] <= '9')
			r = r * 10 + (s[i] - '0');
		else
			throw MainStuffException("not a valid index");
	return r;
}
bool isDivide(char c) {
	return c == '.' || c == '[';
}
void find(const string &targetInfo) {
	//parse info
	int si = 0, ei = 0;
	Data *currentData = &totalData;
	bool parsedTerm = false;
	for (; ei < targetInfo.size(); ++ei) {
		if (targetInfo[ei] == '[') {
			if (!parsedTerm) {
				currentData = currentData->find(targetInfo.substr(si, ei - si));
			}
			if (!currentData) {
				cout << "No such term" << endl;
				return;
			}
			si = ++ei;
			for (; ei < targetInfo.size() && targetInfo[ei] != ']'; ++ei);
			if (!currentData) {
				cout << "No such term" << endl;
				return;
			}
			if (ei == targetInfo.size())
				throw MainStuffException("lack ]");
			if (ei + 1 < targetInfo.size() && (targetInfo[ei + 1] != '[') && (targetInfo[ei + 1] != '.'))
				throw MainStuffException("invalid path");
			currentData = currentData->find(toInt(targetInfo, si, ei));
			parsedTerm = true;
			si = ei + 1;
		}
		else if (targetInfo[ei] == '.') {
			if (parsedTerm) {
				parsedTerm = false;
				si = ei + 1;
				continue;
			}
			currentData = currentData->find(targetInfo.substr(si, ei - si));
			if (!currentData) {
				cout << "No such term" << endl;
				return;
			}
			si = ei + 1;
		}

		//if (!isDivide(targetInfo[ei])) continue;
		//currentData = currentData->find(targetInfo.substr(si, ei - si));
		//si = ei + 1;
		//char c = targetInfo[ei];
		//if (c == '[') {
		//	for (; ei < targetInfo.size() && targetInfo[ei] != ']'; ++ei);
		//	if (ei == targetInfo.size())
		//		throw MainStuffException("lack ]");
		//	if (targetInfo[++ei] != '.')
		//		throw MainStuffException("expected '.' after ']'");
		//	si = ei + 1;
		//	currentData = currentData->find(toInt(targetInfo, si, ei));
		//}
		//else if (c == '.');

		//if (!currentData) {
		//	cout << "No such target" << endl;
		//	return;
		//}
	}
	if (si < ei)
		currentData = currentData->find(targetInfo.substr(si, ei - si));
	if (!currentData) {
		cout << "No such target" << endl;
		return;
	}
	cout << currentData->dataToString() << endl;
}

void JSONize(const string &fileName) {
	string fc = totalData.dataToString();
	ofstream f;
	f.open((fileName.substr(0, fileName.size() - 4) + ".json"), std::ofstream::out | std::ofstream::trunc);
	f.write(fc.c_str(), fc.size());
	f.close();
}

Data * Data::find(int index) const
{
	if (t != DataTypes::Array)
		return nullptr;
	if (index > ((ArrayVector *)v)->size() || index < 1)
		throw MainStuffException("index out of range");
	return &((ArrayVector *)v)->at(index - 1);
}