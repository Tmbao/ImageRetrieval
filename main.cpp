#include "configurations.h"

#include "app/appdata.h"
#include "extract/extract.h"
#include "quantize/quantize.h"
#include "query/query.h"
#include "utils/score.h"
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

    debugInfo("Extracting features");

//    Extract features
    app->path.shrink_to_fit();
    app->kp.reserve(app->path.size());
    app->sift.reserve(app->path.size());
    for (string imgPath : app->path) {

        debugVar(imgPath);

        string tmp = imgPath;
        tmp.replace(tmp.size() - 3, 3, "mat");

        string kpPath = kpFolder + "/" + tmp;
        string siftPath = siftFolder + "/" + tmp;
        string tempPath = tempFolder + "/" + tmp;
        imgPath = dataFolder + "/" + imgPath;

        debugVar(kpPath);
        debugVar(siftPath);
        mat _kp, _sift;
        extractFeatures(imgPath, _kp, _sift, kpPath, siftPath, tempPath, true);

        app->kp.push_back(_kp);
        app->sift.push_back(_sift);
    }
}

void quantizeAllData() {

    debugInfo("Quantizing images");

    buildIndex();

    app->weights.reserve(app->path.size());
    app->termID.reserve(app->path.size());

    int nDocs = app->path.size();
    for (int i = 0; i < nDocs; i++) {

        string tmp = app->path[i];
        tmp.replace(tmp.size() - 3, 3, "mat");

        string weightPath = weightFolder + "/" + tmp;
        string termIDPath = termIDFolder + "/" + tmp;

        debugVar(i);
        debugVar(weightPath);
        debugVar(termIDPath);

        vec _weights;
        uvec _termID;

        buildBoW(app->sift[i], _weights, _termID, weightPath, termIDPath, true);
        
//      Insert to inverted index
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
        extractFeatures(imgPath, _kp, _sift, kpPath, siftPath, tempPath, true);

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
	    //cout << score.score[rankedList[i]] << "\n";
}
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
