#ifndef APPDATA_H_INCLUDED
#define APPDATA_H_INCLUDED


#include "../configurations.h"
#include "../utils/invertedindex.h"


class AppData {
public:

    InvertedIndex ivt;
    vector<string> path;
    // vector<mat> kp, sift;
    // vector<vec> weights;
    // vector<uvec> termID;

private:

    static AppData* _instance;

    AppData(): ivt(number_of_visual_words) {}
    AppData(AppData const &) {}
    AppData& operator = (AppData const &) {}

public:

    static AppData* getInstance() {
        if (_instance == NULL)
            _instance = new AppData();

        return _instance;
    }

};

AppData* AppData::_instance = NULL;


#endif