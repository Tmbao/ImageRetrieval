#ifndef SCORE_H_INCLUDED
#define SCORE_H_INCLUDED

#include "../configurations.h"


class Score {
public:
    vector<double> score;

public:
    Score(vector<double> _score): score(_score) {}

    bool operator() (int i, int j) {
        return score[i] > score[j];
    }
};

#endif
