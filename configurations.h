#ifndef CONFIGURATION_H_INCLUDED
#define CONFIGURATION_H_INCLUDED


#include <bits/stdc++.h>
using namespace std;

#include <armadillo>
using namespace arma;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/flann/flann.hpp>
#include <opencv2/flann/hdf5.h>
#include <boost/filesystem.hpp>

#include <dirent.h>

#include "/utils/hesaff"


const int number_of_visual_words = 1000000;
const double EPS = 1e-9;

const string featureConfig = "-hesaff -sift -noangle";
const string computeDescriptorPath = "./utils/feature_detector/compute_descriptors_64bit.ln";

const string codebookFile = "data/oxbuilding_codebook_l2_1000000_24464227_128_50it.hdf5";
const string indexFile = "data/flannIndex.hdf5";

const string dataFolder = "data/images";
const string queryFolder = "data/queries";
const string rankedListFolder = "data/rankedlists";
const string kpFolder = "data/Kp";
const string siftFolder = "data/SIFT";
const string weightFolder = "data/weight";
const string termIDFolder = "data/termID";
const string apFolder = "data/ap";
const string tempFolder = "data/temp";


// Geometric verification parameters
#define TOLERANCE_1 80
#define TOLERANCE_2 30
#define TOLERANCE_3 40
#define MIN_INLIERS 6
#define N_ITERATIONS 3

// Some macros
#define sqr(x) (x) * (x)
#define debugInfo(x) cerr << x << endl
#define debugVar(x) cerr << #x << " = " << x << endl


#endif
