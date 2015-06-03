#include <bits/stdc++.h>
#include "configurations.h"

using namespace std;

int main() {
	DIR *dir = opendir(apFolder.c_str());
	double mAP = 0;
	int cnt = 0;
    while (dirent *pdir = readdir(dir)) {
        string fName = pdir->d_name;
        ifstream fin(fName);
        double ap;

        fin >> ap;
        fin.close();
        cout << ap;

        ++cnt;
        mAP += ap;
    }

    cout << "mAP:" << mAP / cnt << endl;
}