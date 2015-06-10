#ifndef INVERTEDINDEX_H_INCLUDED
#define INVERTEDINDEX_H_INCLUDED


#include "../configurations.h"


struct InvertedIndex {

    int nWords, nDocs;

    vector<vector<int>> index;
    vector<vector<double>> frequency;
    vector<double> sumFrequency;
    vector<vector<double>> tfidf;

    bool isBuild;

    InvertedIndex(): isBuild(false) {}
    InvertedIndex(int n): index(n), frequency(n), tfidf(n), nWords(n), nDocs(0), isBuild(false) {}

    void buildTfidf() {

        debugInfo("Building TFIDF based on inverted index");


        debugInfo("Allocating tfidf");
        for (int i = 0; i < nWords; i++) 
            tfidf[i].resize(index[i].size());

        debugInfo("Computing sumFrequency");
        sumFrequency.resize(nDocs, 0);
        for (int i = 0; i < nWords; i++)
            for (int j = 0; j < index[i].size(); j++)
                sumFrequency[index[i][j]] += frequency[i][j];

        debugInfo("Computing tfidf");
        for (int i = 0; i < nWords; i++) {
            
            if (index[i].size() == 0)
                debugVar(i);

            double idf = log(double(nDocs) / (index[i].size()));

            for (int j = 0; j < index[i].size(); j++) {
                double tf = sqrt(frequency[i][j] / sumFrequency[index[i][j]] + 1e-8);

                tfidf[i][j] = tf * idf;
		            if (std::isnan(tf * idf))
			             cout << "tfidf is nan\n";
            }
        }

        isBuild = true;

        debugInfo("Finish");
    }

    vector<double> makeQueryTfidf(vec freq, uvec termId) {

        if (!isBuild) 
            throw logic_error("You must perform buildTfidf before makeQueryTfidf");

        vector<double> qTfidf(nWords);

        vector <int> rawFreq(nWords);
        vector<double> qFrequency(nWords, 0);
        double qSumFrequency = 0;
        for (int i = 0; i < termId.n_elem; i++) {
            qFrequency[termId[i]] += freq[i];
            rawFreq[termId[i]]++;
        }
        for (int i = 0; i < nWords; i++) {
            qFrequency[i] /= sqrt(fabs(rawFreq[i] + 1));
            qSumFrequency += qFrequency[i];
        }

        for (int i = 0; i < nWords; i++) {
            double idf = log(double(nDocs) / (index[i].size()));
            double tf = sqrt(fabs(qFrequency[i] / qSumFrequency));

            qTfidf[i] = tf * idf;
            if (std::isnan(tf))
               cout << qFrequency[i] << " " << qSumFrequency << " nan\n";
        }

        return qTfidf;
    }

    void add(vec freq, uvec termId, int docId) {
        nDocs++;

        debugInfo("Adding document to inverted index");
        debugVar(docId);

        vector <int> rawFreq(nWords);
        for (int i = 0; i < termId.n_elem; i++) {
            if (index[termId[i]].empty() || index[termId[i]].back() != docId) {
                index[termId[i]].push_back(docId);
                frequency[termId[i]].push_back(0);
            }
            rawFreq[termId[i]]++;
            frequency[termId[i]].back() += freq[i];
        }

        for (int i = 0; i < nWords; ++i)
            if (!index[i].empty() && index[i].back() == docId)
                frequency[i].back() /= sqrt(rawFreq[i] + 1);

        isBuild = false;

        debugInfo("Finish");
    }
};


#endif
