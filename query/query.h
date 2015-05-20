#ifndef QUERY_H_INCLUDED
#define QUERY_H_INCLUDED


#include "../configurations.h"
#include "../app/appdata.h"


vector<double> computeAllDistances(vector<double> qTfidf) {
    AppData *app = AppData::getInstance();

    vector<double> distances(app->ivt.nDocs);
    for (int i = 0; i < app->ivt.nWords; i++) {
        if (abs(qTfidf[i]) < eps)
            continue;
        for (int j = 0; j < app->ivt.index[i].size(); j++)
            distances[app->ivt.index[i][j]] += qTfidf[i] * app->ivt.tfidf[i][j];
    }
    
    return distances;
}


#endif
