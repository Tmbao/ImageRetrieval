#ifndef IMAGE_RETRIEVAL_QUERY_H
#define IMAGE_RETRIEVAL_QUERY_H


#include "../configurations.h"
#include "../app/app_data.h"


vector<double> compute_all_distances(vector<double> q_tfidf) {
    app_data *app = app_data::get_instance();

    vector<double> distances(app->ivt.n_docs);
    for (int i = 0; i < app->ivt.n_words; i++) 
        for (int j = 0; j < app->ivt.index[i].size(); j++)
            distances[app->ivt.index[i][j]] += q_tfidf[i] * app->ivt.tfidf[i][j];
    
    return distances;
}


#endif IMAGE_RETRIEVAL_QUERY_H
