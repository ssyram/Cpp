#include "LaxAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include <iostream>
#include "MainStuff.h"
using std::cout;
using std::cerr;
using std::cin;
using std::endl;

void printLines() {
	for (int i = 0; i < parsedLines.size(); ++i)
		parsedLines[i].printLine();
}

inline void parse(const string &fileName) {
	LexAnalyzer l(fileName);
	SyntaxAnalyzer s;
}
inline void goParse(const char *fileName) {
	if (isValidFile(fileName))
		parse(fileName);
	else
		throw MainStuffException("Not a valid file name");
}

void chooseWay(int argc, char *argv[]) {
	if (argc == 2)
		goParse(argv[1]);
	else if (argc == 3)
		if (!strcmp(argv[1], "-parse"))
			goParse(argv[1]);
		else if ((!strcmp(argv[1], "-JSON")) || (!strcmp(argv[1], "-json"))) {
			goParse(argv[2]);
			JSONize(argv[2]);
			cout << "JSON file created" << endl;
		}
		else
			throw MainStuffException();
	else if (argc == 4 && (!strcmp(argv[1], "-find"))) {
		goParse(argv[3]);
		find(argv[2]);
	}
	else
		throw MainStuffException();
}

int main(int argc, char *argv[]) {
	//argc = 3;
	//char *a[] = { argv[0], "-json", "sample.yml" };
	//argv = a;
	//for (int i = 0; i < argc; ++i)
	//	cout << argv[i] << endl;
	//cout << endl;

	try
	{
		chooseWay(argc, argv);
	}
	catch (ParseException &pe)
	{
		cerr << pe.getInfo() << endl;
	}
	catch (MainStuffException &mse)
	{
		cerr << mse.info << endl;
	}

	//printLines();

	//cout << endl;

	//try {
	//	chooseWay(argc, argv);
	//}
	//catch (MainStuffException &mse) {
	//	cout << mse.info << endl;
	//}
	//catch (ParseException &pe) {
	//	cout << pe.getInfo() << endl;
	//}
}