#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "../configurations.h"


string getFileBaseName(string fileName) {
    return fileName.substr(0, fileName.rfind("."));
}

int parseInt(string s) {
    return atoi(s.c_str());
}

string vectorToJson(vector<string> v, string name) {
    string json = "{\n\t" + name + ": [\n";
    for (int i = 0; i < v.size(); i++) {
        json += "\t\t" + v[i];
        if (i + 1 < v.size())
            json += ",";
        json += "\n";
    }
    json += "\t]\n}";
    return json;
}

#endif
