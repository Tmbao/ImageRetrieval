#ifndef IMAGE_RETRIEVAL_INVERTED_INDEX_H
#define IMAGE_RETRIEVAL_INVERTED_INDEX_H


#include "../configurations.h"


struct inverted_index {

    int n_words, n_docs;

    vector<vector<int>> index;
    vector<vector<double>> frequency;
    vector<double> sum_frequency;
    vector<vector<double>> tfidf;

    bool is_build;

    inverted_index(): is_build(false) {}
    inverted_index(int n): index(n), frequency(n), tfidf(n), n_words(n), n_docs(0), is_build(false) {}

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
                double tf = sqrt(frequency[i][j] / sum_frequency[index[i][j]]);

                tfidf[i][j] = tf * idf;
            }
        }

        is_build = true;
    }

    vector<double> make_query_tfidf(vec freq, uvec term_id) {

        if (!is_build) 
            throw logic_error("You must perform build_tfidf before make_query_tfidf");

        vector<double> q_tfidf(n_words);

        vector<double> q_frequency(n_words, 0);
        double q_sum_frequency = 0;
        for (int i = 0; i < term_id.n_elem; i++) {
            q_frequency[term_id[i]] += freq[i];
            q_sum_frequency += freq[i];
        }

        for (int i = 0; i < n_words; i++) {
            double idf = log(double(n_docs) / index[i].size());
            double tf = sqrt(q_frequency[i] / q_sum_frequency);

            q_tfidf[i] = tf * idf;
        }

        return q_tfidf;
    }

    void add(vec freq, uvec term_id, int doc_id) {
        n_docs++;
        for (int i = 0; i < term_id.n_elem; i++) {
            if (index[term_id[i]].empty() || index[term_id[i]].back() != doc_id) {
                index[term_id[i]].push_back(doc_id);
                frequency[term_id[i]].push_back(0);
            }
            frequency[term_id[i]].back() += freq[i];
        }

        is_build = false;
    }
};


#endif IMAGE_RETRIEVAL_INVERTED_INDEX_H
