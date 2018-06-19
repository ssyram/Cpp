#pragma once

#include <string>
#include <iostream>
using std::cout;
using std::endl;
using std::string;

struct MainStuffException {
	string info;
	MainStuffException(const string &info = "Format: yamlite [-parse/-json/-find {condition}] ymlFileName") : info(info) {}
};

bool isValidFile(const string &fileName);

void find(const string &targetInfo);

void JSONize(const string &fileName);