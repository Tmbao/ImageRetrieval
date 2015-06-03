#include <bits/stdc++.h>
#include "dirent.h"

using namespace std;

const string apFolder = "data/ap";

int main() {
    DIR *dir = opendir(apFolder.c_str());
    double mAP = 0;
    int cnt = 0;
    while (dirent *pdir = readdir(dir)) {
        string fName = pdir->d_name;

        if (fName == "..")
            continue;

        ifstream fin(fName.c_str());
        double ap;

        fin >> ap;
        fin.close();
        cout << ap;

        ++cnt;
        mAP += ap;
    }

    cout << "mAP:" << mAP / cnt << endl;
}
