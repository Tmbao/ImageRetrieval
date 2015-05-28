#ifndef QUERY_H_INCLUDED
#define QUERY_H_INCLUDED


#include "../configurations.h"
#include "../app/appdata.h"


vector<double> computeAllScores(vector<double> qTfidf) {
    AppData *app = AppData::getInstance();

    vector<double> scores(app->ivt.nDocs);
    for (int i = 0; i < app->ivt.nWords; i++) {
        if (abs(qTfidf[i]) < EPS)
            continue;
        for (int j = 0; j < app->ivt.index[i].size(); j++)
            scores[app->ivt.index[i][j]] += qTfidf[i] * app->ivt.tfidf[i][j];
    }
    
    return scores;
}

#endif
