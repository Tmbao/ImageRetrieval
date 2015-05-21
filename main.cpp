#include "configurations.h"

#include "app/appdata.h"
#include "extract/extract.h"
#include "quantize/quantize.h"
#include "query/query.h"
#include "utils/distance.h"
#include "utils/utils.h"

AppData *app;

void extractAll() {

//    Get list of image files
    DIR *dir = opendir(dataFolder.c_str());
    while (dirent *pdir = readdir(dir)) {
        string fName = pdir->d_name;
        if (fName[0] == '.') continue;

        app->path.push_back(fName);
    }

//    Extract features
    app->path.shrink_to_fit();
    app->kp.reserve(app->path.size());
    app->sift.reserve(app->path.size());
    for (string imgPath : app->path) {
        imgPath = dataFolder + "/" + imgPath;
        mat _kp, _sift;
        extractFeatures(imgPath, _kp, _sift);

        app->kp.push_back(_kp);
        app->sift.push_back(_sift);
    }
}

void quantizeAllData() {
    buildIndex();

    app->weights.reserve(app->path.size());
    app->termID.reserve(app->path.size());

    int nDocs = app->path.size();
    for (int i = 0; i < nDocs; i++) {
        mat _weights;
        umat _termID;
        buildBoW(app->sift[i], _weights, _termID);

//        Insert to inverted index
        app->ivt.add(_weights, _termID, i);

        app->weights.push_back(_weights);
        app->termID.push_back(_termID);
    }

//    Build TFIDF
    app->ivt.buildTfidf();
}

void processAllQueries() {

    vector<string> queryPath;
    vector<mat> queryKp, querySift;

//    Get list of image files
    DIR *dir = opendir(queryFolder.c_str());
    while (dirent *pdir = readdir(dir)) {
        string fName = pdir->d_name;
        if (fName[0] == '.') continue;

        queryPath.push_back(fName);
    }

//    Extract features
    queryKp.reserve(queryPath.size());
    querySift.reserve(queryPath.size());
    for (string imgPath : queryPath) {
        mat _kp, _sift;
        extractFeatures(imgPath, _kp, _sift);

        queryKp.push_back(_kp);
        querySift.push_back(_sift);
    }

//    Builf TFIDF and compute ranked list
    int nQueries = queryPath.size();
    int nDocs = app->path.size();
    for (int i = 0; i < nQueries; i++) {
        mat _weights;
        umat _termID;

        buildBoW(querySift[i], _weights, _termID);

        vector<double> qTfidf = app->ivt.makeQueryTfidf(_weights, _termID);

        Distance distance(computeAllDistances(qTfidf));

        vector<int> rankedList(nDocs);
        for (int i = 0; i < nDocs; i++)
            rankedList[i] = i;
        sort(rankedList.begin(), rankedList.end(), distance);

        string rankedListPath = rankedListFolder + "/" + getFileBaseName(queryPath[i]) + ".txt";
        FILE *rankedListFile = fopen(rankedListPath.c_str(), "w");
        for (int i = 0; i < nDocs; i++)
            fprintf(rankedListFile, "%s\n", getFileBaseName(app->path[rankedList[i]]).c_str());
        fclose(rankedListFile);
    }
}

int main() {

    app = AppData::getInstance();

    extractAll();
    quantizeAllData();
    processAllQueries();

    return 0;
}
