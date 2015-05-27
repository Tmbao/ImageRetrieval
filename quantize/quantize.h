#ifndef QUANTIZE_H_INCLUDED
#define QUANTIZE_H_INCLUDED

#include "../configurations.h"


const double deltaSqr = 6250;
const int nKdTree = 8;
const int nChecks = 800;
const int dataKnn = 1;
const int queryKnn = 3;

void buildIndex() {
    cvflann::Matrix<double> dataset;

    cvflann::load_from_file(dataset, codebookFile, "clusters");

    if (boost::filesystem::exists(indexFile))
        return;

    cvflann::Index<cvflann::L2<double> > treeIndex(dataset, cvflann::KDTreeIndexParams(nKdTree));
    treeIndex.save(indexFile);
}

void buildBoW(const mat &imageDesc, mat &weights, umat &termID, const string &weightPath, const string &termIDPath) {
    if (boost::filesystem::exists(weightPath)) {
        weights.load(weightPath);
        termID.load(termIDPath);
        return;
    }

    double *tmpData = new double[imageDesc.n_elem];
    memcpy(tmpData, imageDesc.memptr(), sizeof(double) * imageDesc.n_elem);
    cvflann::Matrix<double> query(tmpData, imageDesc.n_cols, imageDesc.n_rows);

    cout << query.rows <<" "<<query.cols << endl;

    cvflann::Matrix<int> indices(new int[query.rows*queryKnn], query.rows, queryKnn);
    cvflann::Matrix<double> dists(new double[query.rows*queryKnn], query.rows, queryKnn);

    cout << "Load index file\n";
    cvflann::Index<cvflann::L2<double> > treeIndex(dataset, cvflann::SavedIndexParams(indexFile));
    cout << "Loaded sucessfully\n";

    cout << "Start knn search\n";
    treeIndex.knnSearch(query, indices, dists, queryKnn, cvflann::SearchParams(nChecks));
    cout << "knn search sucessfully\n";

    umat bins(queryKnn, query.rows);
    memcpy(bins.memptr(), indices.data, query.rows * queryKnn * sizeof(int));
    mat sqrDists(queryKnn, query.rows);
    memcpy(sqrDists.memptr(), dists.data, query.rows * queryKnn * sizeof(double));

    termID = vectorise(bins, 0);

    weights = exp(-sqrDists / (2 * deltaSqr));
    weights = weights / repmat(sum(weights, 0), weights.n_rows, 1);
    vectorise(weights, 0);

    weights.save(weightPath);
    termID.save(termIDPath);
}

#endif
