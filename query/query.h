#ifndef QUERY_H_INCLUDED
#define QUERY_H_INCLUDED


#include "../configurations.h"
#include "../app/appdata.h"


vector<double> computeAllDistances(vector<double> q_tfidf) {
    AppData *app = AppData::getInstance();

    vector<double> distances(app->ivt.nDocs);
    for (int i = 0; i < app->ivt.nWords; i++)
        for (int j = 0; j < app->ivt.index[i].size(); j++)
            distances[app->ivt.index[i][j]] += q_tfidf[i] * app->ivt.tfidf[i][j];
    
    return distances;
}


#endif QUERY_H_INCLUDED
