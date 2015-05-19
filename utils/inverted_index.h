#ifndef IMAGE_RETRIEVAL_INVERTED_INDEX_H
#define IMAGE_RETRIEVAL_INVERTED_INDEX_H


#include "../configurations.h"


struct inverted_index {

    int n_words, n_docs;

    vector<vector<int>> index;
    vector<vector<double>> frequency;
    vector<double> sum_frequency;
    vector<vector<double>> tfidf;



    inverted_index() {}
    inverted_index(int n): index(n), frequency(n), tfidf(n), n_words(n), n_docs(0) {}

    void build_tfidf() {
        for (int i = 0; i < n_words; i++) 
            tfidf[i].resize(index[i].size());

        sum_frequency.resize(n_docs, 0);
        for (int i = 0; i < n_words; i++)
            for (int j = 0; j < index[i].size(); j++)
                sum_frequency[index[i][j]] += frequency[i][j];

        for (int i = 0; i < n_words; i++) {
            double idf = log(double(n_docs) / index[i].size());

            for (int j = 0; j < index[i].size(); j++) {
                double tf = frequency[i][j] / sum_frequency[index[i][j]];

                tfidf[i][j] = tf * idf;
            }
        }
    }

    void add(vec freq, uvec term_id, int doc_id) {
        n_docs++;
        for (int i = 0; i < term_id.n_elem; i++) {
            index[term_id[i]].push_back(doc_id);
            frequency[term_id[i]].push_back(freq[i]);
        }
    }
};


#endif IMAGE_RETRIEVAL_INVERTED_INDEX_H