#ifndef SCORE_H_INCLUDED
#define SCORE_H_INCLUDED

#include "../configurations.h"


class Distance {
private:
    vector<double> distances;

public:
    Distance(vector<double> _distances): distances(_distances) {}

    bool operator() (int i, int j) {
        return distances[i] < distances[j];
    }
};

#endif
