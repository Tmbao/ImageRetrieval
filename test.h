#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED


#include "configurations.h"
#include "app/appdata.h"
#include "extract/extract.h"
#include "quantize/quantize.h"
#include "query/query.h"
#include "utils/score.h"
#include "utils/utils.h"


void processAllQueries() {

    AppData *app = AppData::getInstance();

    vector<string> queryPath;
    vector<mat> queryKp, querySift;

//    Get list of image files
    DIR *dir = opendir(queryFolder.c_str());
    while (dirent *pdir = readdir(dir)) {
        string fName = pdir->d_name;
        if (fName[0] == '.') continue;

        queryPath.push_back(fName);
    }

    debugInfo("Extracting queries' features");

//    Extract features
    queryKp.reserve(queryPath.size());
    querySift.reserve(queryPath.size());
    for (string imgPath : queryPath) {

        debugVar(imgPath);

        string tmp = imgPath;
        tmp.replace(tmp.size() - 3, 3, "mat");

        string kpPath = kpFolder + "/" + tmp;
        string siftPath = siftFolder + "/" + tmp;
        string tempPath = tempFolder + "/" + tmp;
        imgPath = queryFolder + "/" + imgPath;

        mat _kp, _sift;
        extractFeatures(imgPath, _kp, _sift, kpPath, siftPath, tempPath);

        queryKp.push_back(_kp);
        querySift.push_back(_sift);
    }

    debugInfo("Computing queries' ranked list");

//    Builf TFIDF and compute ranked list
    int nQueries = queryPath.size();
    int nDocs = app->path.size();
    for (int i = 0; i < nQueries; i++) {

        debugVar(i);

        vec _weights;
        uvec _termID;

        string tmp = queryPath[i];
        tmp.replace(tmp.size() - 3, 3, "mat");

        string weightPath = weightFolder + "/" + tmp;
        string termIDPath = termIDFolder + "/" + tmp;

        debugInfo("Building query BOW");
        buildBoW(querySift[i], _weights, _termID, weightPath, termIDPath, true);
        
        debugInfo("Building query tfidf");
        vector<double> qTfidf = app->ivt.makeQueryTfidf(_weights, _termID);

        debugInfo("Computing scores");
        Score score(computeAllScores(qTfidf));

        debugInfo("Intializing ranked list");
        vector<int> rankedList(nDocs);
        for (int i = 0; i < nDocs; i++)
            rankedList[i] = i;
        debugInfo("Sorting ranked list");
        sort(rankedList.begin(), rankedList.end(), score);

        debugInfo("Outputing ranked list");
        string rankedListPath = rankedListFolder + "/" + getFileBaseName(queryPath[i]) + ".txt";
        FILE *rankedListFile = fopen(rankedListPath.c_str(), "w");
        for (int i = 0; i < nDocs; i++) {
            fprintf(rankedListFile, "%s\n", getFileBaseName(app->path[rankedList[i]]).c_str());
        }
        fclose(rankedListFile);
    }
}


void runTest() {
    processAllQueries();

    // !!!TODO: Compute result map
}

#endif
