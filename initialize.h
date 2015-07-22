#ifndef INITIALIZE_H_INCLUDED
#define INITIALIZE_H_INCLUDED


#include "configurations.h"
#include "app/appdata.h"
#include "extract/extract.h"
#include "quantize/quantize.h"


void extractAndQuantizeAll() {

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

    buildIndex(false);

    for (string imgPath : app->path) {

        debugVar(imgPath);

        string tmp = imgPath;
        tmp.replace(tmp.size() - 3, 3, "mat");

        string kpPath = kpFolder + "/" + tmp;
        string siftPath = siftFolder + "/" + tmp;
        string tempPath = tempFolder + "/" + tmp;
        string weightPath = weightFolder + "/" + tmp;
        string termIDPath = termIDFolder + "/" + tmp;

        imgPath = dataFolder + "/" + imgPath;

        debugVar(imgPath);
        debugVar(kpPath);
        debugVar(siftPath);
        debugVar(weightPath);
        debugVar(termIDPath);

        mat _kp, _sift;
        extractFeatures(imgPath, _kp, _sift, kpPath, siftPath, tempPath, false);

        vec _weights;
        uvec _termID;

        buildBoW(_sift[i], _weights, _termID, weightPath, termIDPath, false);
        
//        Insert to inverted index
        app->ivt.add(_weights, _termID, i);
    }

    //    Build TFIDF
    app->ivt.buildTfidf();
}

#endif
