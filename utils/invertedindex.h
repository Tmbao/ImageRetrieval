#ifndef INVERTEDINDEX_H_INCLUDED
#define INVERTEDINDEX_H_INCLUDED


#include "../configurations.h"


struct InvertedIndex {

    int nWords, nDocs;

    vector<vector<int> > index;
    vector<vector<double> > frequency;
    vector<double> sumFrequency;
    vector<vector<double> > tfidf;

    bool isBuild;

    InvertedIndex(): isBuild(false) {}
    InvertedIndex(int n): index(n), frequency(n), tfidf(n), nWords(n), nDocs(0), isBuild(false) {}

    void buildTfidf() {
        for (int i = 0; i < nWords; i++) 
            tfidf[i].resize(index[i].size());

        sumFrequency.resize(nDocs, 0);
        for (int i = 0; i < nWords; i++)
            for (int j = 0; j < index[i].size(); j++)
                sumFrequency[index[i][j]] += frequency[i][j];

        for (int i = 0; i < nWords; i++) {
            double idf = log(double(nDocs) / index[i].size());

            for (int j = 0; j < index[i].size(); j++) {
                double tf = sqrt(frequency[i][j] / sumFrequency[index[i][j]]);

                tfidf[i][j] = tf * idf;
            }
        }

        isBuild = true;
    }

    vector<double> makeQueryTfidf(vec freq, uvec termId) {

        if (!isBuild) 
            throw logic_error("You must perform buildTfidf before makeQueryTfidf");

        vector<double> qTfidf(nWords);

        vector<double> qFrequency(nWords, 0);
        double qSumFrequency = 0;
        for (int i = 0; i < termId.n_elem; i++) {
            qFrequency[termId[i]] += freq[i];
            qSumFrequency += freq[i];
        }

        for (int i = 0; i < nWords; i++) {
            double idf = log(double(nDocs) / index[i].size());
            double tf = sqrt(qFrequency[i] / qSumFrequency);

            qTfidf[i] = tf * idf / sqrt(index[i].size());
        }

        return qTfidf;
    }

    void add(vec freq, uvec termId, int docId) {
        vector <int> raw_freq(nWords);
        nDocs++;
        for (int i = 0; i < termId.n_elem; i++) {
            if (index[termId[i]].empty() || index[termId[i]].back() != docId) {
                index[termId[i]].push_back(docId);
                frequency[termId[i]].push_back(0);
            }
            raw_freq[termId[i]]++;
            frequency[termId[i]].back() += freq[i];
        }

        for (int i = 0; i < nWords; ++i)
            if (index[i].back() == docId)
                frequency[i].back() /= sqrt(raw_freq[i]);

        isBuild = false;
    }
};


#endif INVERTEDINDEX_H_INCLUDED
