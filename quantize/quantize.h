#ifndef QUANTIZE_H_INCLUDED
#define QUANTIZE_H_INCLUDED

#include <opencv2/flann/flann.hpp>
#include <opencv2/flann/hdf5.h>
#include <armadillo>
#include <string>
#include <fstream>

using namespace std;
using namespace arma;

const double deltaSqr = 6250;
const int nKdTree = 8;
const int nChecks = 800;
const int dataKnn = 1;
const int queryKnn = 3;

const string codebookFile = "oxbuilding_codebook_l2_1000000_24464227_128_50it_new.hdf5";
const string indexFile = "flannIndex.hdf5";

cvflann::Matrix<double> dataset;
cvflann::Index<cvflann::L2<double> > *treeIndex;

bool exist(string filename) {
    ifstream fin(filename.c_str());
    if (!fin.good()) {
        fin.close();
        return 1;
    }
    else {
        fin.close();
        return 0;
    }
}

void buildIndex() {
    cvflann::Matrix<double> dataset;

    cvflann::load_from_file(dataset, codebookFile, "clusters");

    cvflann::IndexParams *indexParams;

    if (exist(indexFile))
        indexParams = new cvflann::SavedIndexParams(indexFile);
    else
        indexParams = new cvflann::KDTreeIndexParams(nKdTree);

    treeIndex = new cvflann::Index<cvflann::L2<double> > (dataset, *indexParams);


}

void buildBoW(const mat &imageDesc, mat &weights, umat &termID) {
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

    termID = vectorise(bins, 0);
            
    weights = exp(-sqrDists / (2 * deltaSqr));
    weights = weights / repmat(sum(weights, 0), weights.n_rows, 1);
    vectorise(weights, 0);
}

#endif // QUANTIZE_H_INCLUDED
