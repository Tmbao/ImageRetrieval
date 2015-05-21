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

#include <dirent.h>


const int number_of_visual_words = 1000000;
const double EPS = 1e-9;

const string featureConfig = "-hesaff -sift -noangle";
const string computeDescriptorPath = "./feature_detector/compute_descriptors_64bit.ln";

const string codebookFile = "data/oxbuilding_codebook_l2_1000000_24464227_128_50it_new.hdf5";
const string indexFile = "data/flannIndex.hdf5";

const string dataFolder = "data/images";
const string queryFolder = "data/queries";
const string rankedListFolder = "data/rankedlists";

#endif
