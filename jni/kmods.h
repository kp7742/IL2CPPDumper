#ifndef KMODS_H
#define KMODS_H

#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <getopt.h>

#include "Log.h"
#include "Offsets.h"
#include "Process.h"
#include "Memory.h"
#include "MetaData.h"
#include "Dumper.h"

static const char* lib_name = "libil2cpp.so";

bool isStartWith(string str, const char* check) {
	return (str.rfind(check, 0) == 0);
}

bool isEqual(char* s1, const char* s2) {
	return (strcmp(s1, s2) == 0);
}

bool isEqual(string s1, const char* check) {
	string s2(check);
	return (s1 == s2);
}

bool isEqual(string s1, string s2) {
	return (s1 == s2);
}

bool isContain(string str, const char* check) {
	size_t found = str.find(check);
	return (found != string::npos);
}

#endif
