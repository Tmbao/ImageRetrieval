#include "configurations.h"

#include "app/appdata.h"
#include "extract/extract.h"
#include "quantize/quantize.h"
#include "query/query.h"
#include "utils/distance.h"
#include "utils/utils.h"

AppData *app;

#define debugInfo(x) cerr << x << endl
#define debugVar(x) cerr << #x << " = " << x << endl

void extractAll() {

//    Get list of image files
    vector <string> paths;
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

        imgPath = dataFolder + "/" + imgPath;
        mat _kp, _sift;
        extractFeatures(imgPath, _kp, _sift);

        app->kp.push_back(_kp);
        app->sift.push_back(_sift);
    }
}