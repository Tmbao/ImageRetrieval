#ifndef INITIALIZE_H_INCLUDED
#define INITIALIZE_H_INCLUDED


#include "configurations.h"
#include "app/appdata.h"
#include "extract/extract.h"
#include "quantize/quantize.h"


void extractAll() {

    AppData *app = AppData::getInstance();

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
    // app->kp.reserve(app->path.size());
    // app->sift.reserve(app->path.size());
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
        extractFeatures(imgPath, _kp, _sift, kpPath, siftPath, tempPath);

        // app->kp.push_back(_kp);
        // app->sift.push_back(_sift);
    }
}

void quantizeAllData() {

    AppData *app = AppData::getInstance();

    debugInfo("Quantizing images");

    buildIndex(true);

    // app->weights.reserve(app->path.size());
    // app->termID.reserve(app->path.size());

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

        buildBoW(app->sift[i], _weights, _termID, weightPath, termIDPath, false);
        
//        Insert to inverted index
        app->ivt.add(_weights, _termID, i);

        // app->weights.push_back(_weights);
        // app->termID.push_back(_termID);
    }

//    Build TFIDF
    app->ivt.buildTfidf();
}

#endif
