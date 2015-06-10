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

#include "local_configurations.h"


static const int number_of_visual_words = 1000000;
static const double EPS = 1e-9;

static const string featureConfig = "-hesaff -sift -noangle";
static const string computeDescriptorPath = "./utils/hesaff/hesaff";

// static const string codebookFile = "data/oxbuilding_codebook_l2_1000000_24464227_128_50it.hdf5";
static const string codebookFile = "data/Clustering_l2_1000000_13516675_128_50it.hdf5";
static const string indexFile = "data/flannIndex.hdf5";

static const string dataFolder = "data/images";
static const string queryFolder = "data/queries";
static const string rankedListFolder = "data/rankedlists";
static const string kpFolder = "data/Kp";
static const string siftFolder = "data/SIFT";
static const string weightFolder = "data/weight";
static const string termIDFolder = "data/termID";
static const string apFolder = "data/ap";
static const string tempFolder = "data/temp";

static const int loopSleepTime = 1000;


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
