#ifndef QUANTIZE_H_INCLUDED
#define QUANTIZE_H_INCLUDED

#include "../configurations.h"


const double deltaSqr = 6250;
const int nKdTree = 8;
const int nChecks = 800;
const int dataKnn = 1;
const int queryKnn = 3;

cvflann::Matrix<double> dataset;
cvflann::Index<cvflann::L2<double>> *treeIndex;

void buildIndex(bool force = false) {
    cvflann::load_from_file(dataset, codebookFile, "clusters");

    cvflann::IndexParams *indexParams;

    if (!force && boost::filesystem::exists(indexFile))
        indexParams = new cvflann::SavedIndexParams(indexFile);
    else
        indexParams = new cvflann::KDTreeIndexParams(nKdTree);

    treeIndex = new cvflann::Index<cvflann::L2<double>> (dataset, *indexParams);
    treeIndex->buildIndex();
    treeIndex->save(indexFile);
}

void buildBoW(const mat &imageDesc, vec &_weights, uvec &_termID, const string &weightPath, const string &termIDPath, bool force = false) {
    if (!force && boost::filesystem::exists(weightPath)) {
        _weights.load(weightPath);
        _termID.load(termIDPath);
        return;
    }

    double *tmpData = new double[imageDesc.n_elem];
    memcpy(tmpData, imageDesc.memptr(), sizeof(double) * imageDesc.n_elem);
    cvflann::Matrix<double> query(tmpData, imageDesc.n_cols, imageDesc.n_rows);

    cvflann::Matrix<int> indices(new int[query.rows*queryKnn], query.rows, queryKnn);
    cvflann::Matrix<double> dists(new double[query.rows*queryKnn], query.rows, queryKnn);

    treeIndex->knnSearch(query, indices, dists, queryKnn, cvflann::SearchParams(nChecks));

    umat bins(queryKnn, query.rows);
    memcpy(bins.memptr(), indices.data, query.rows * queryKnn * sizeof(int));
    mat sqrDists(queryKnn, query.rows);
    memcpy(sqrDists.memptr(), dists.data, query.rows * queryKnn * sizeof(double));

    _termID = vectorise(bins, 0);

    mat weights = exp(-sqrDists / (2 * deltaSqr));
    weights = weights / repmat(sum(weights, 0), weights.n_rows, 1);
    _weights = vectorise(weights, 0);

    _weights.save(weightPath);
    _termID.save(termIDPath);
}

#endif
