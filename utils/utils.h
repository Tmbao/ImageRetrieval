#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "../configurations.h"


string getFileBaseName(string fileName) {
    return fileName.substr(0, fileName.rfind("."));
}

int parseInt(string s) {
    return atoi(s.c_str());
}

#endif
