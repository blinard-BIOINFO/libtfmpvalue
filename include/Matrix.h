/*
 *  Matrix.h
 *  pvalue
 *
 *  Created by Jean-Stéphane Varré on 02/07/07.
 *  Copyright 2007 LIFL-USTL-INRIA. All rights reserved.
 *
 */


#ifndef TFM_PVALUE_MATRIX_H
#define TFM_PVALUE_MATRIX_H

#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <unordered_map>
#include <absl/container/btree_map.h>

#include "FileException.h"
#include "ParseException.h"

using namespace std;

#define ROUND_TO_INT(n) ((int64_t)floor(n))


//#define PRINTLOGRATIO

class Matrix {


public:

    // used for efficiency tests
    int64_t totalMapSize=0;
    int64_t totalOp=0;

    vector<vector<double>> mat; // the matrix as it is stored in the matrix file
    int length=0;
    double granularity; // the real granularity used, greater than 1
    vector<vector<int64_t>> matInt; // the discrete matrix with offset
    double errorMax=0;
    vector<int64_t> offsets; // offset of each column
    int64_t offset; // sum of offsets
    vector<int64_t> minScoreColumn; // min discrete score at each column
    vector<int64_t> maxScoreColumn; // max discrete score at each column
    vector<int64_t> sum;
    int64_t minScore=0;  // min total discrete score (normally 0)
    int64_t maxScore=0;  // max total discrete score
    int64_t scoreRange=0;  // score range = max - min + 1
    vector<int64_t> bestScore;
    vector<int64_t> worstScore;
    double background[4] = {0.25,0.25,0.25,0.25};

    Matrix(size_t length);
    Matrix(size_t length, double pA, double pC, double pG, double pT);

    /**
    * Transforms the initial matrix into an integer and offseted matrix.
    */
    void computesIntegerMatrix (double granularity, bool sortColumns = true);

    /**
    * Computes the pvalue associated with the threshold score requestedScore.
    */
    void lookForPvalue (int64_t requestedScore, int64_t min, int64_t max, double *pmin, double *pmax);

    /**
    * Computes the score associated with the pvalue requestedPvalue.
    */
    int64_t lookForScore (int64_t min, int64_t max, double requestedPvalue, double *rpv, double *rppv);

    /**
    * Computes the distribution of scores between score min and max as the DP algrithm proceeds
    * but instead of using a table we use a map to avoid computations for scores that cannot be reached
    */
    std::vector<absl::btree_map<int64_t, double>> calcDistribWithMapMinMax (int64_t min, int64_t max);


    /**
    * Computes the pvalue for a given score and at a fixed granularity
    */
    int64_t fastPvalue (Matrix *m, int64_t alpha);


    void toLogOddRatio ();
    // computes the complete score distribution between score min and max
    void showDistrib (int64_t min, int64_t max);
  
}; /* Matrix */




#endif //TFM_PVALUE_MATRIX_H
