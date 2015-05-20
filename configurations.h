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
const double eps = 1e-9;

const string featureConfig = "-hesaff -sift -noangle";
const string computeDescriptorPath = "compute_descriptors.exe";

const string codebookFile = "oxbuilding_codebook_l2_1000000_24464227_128_50it_new.hdf5";
const string indexFile = "flannIndex.hdf5";


#endif
