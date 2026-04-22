/*
 *  Matrix.cpp
 *  pvalue
 *
 *  Created by Jean-Stéphane Varré on 02/07/07.
 *  Copyright 2007 LIFL-USTL-INRIA. All rights reserved.
 *
 */


//#define PRINTVERBOSE 
//#define SHOWCERR
//#define VERBOSE


#include "../include/Matrix.h"

Matrix::Matrix(size_t length):
mat(4, std::vector<int64_t>(length)),
matInt(4, std::vector<int64_t>(length)),
offsets(length),
minScoreColumn(length),
maxScoreColumn(length),
sum(length),
bestScore(length),
worstScore(length)
{
    this->length = length;
    granularity = 1.0;
    offset = 0;
}

Matrix::Matrix(size_t length, double pA, double pC, double pG, double pT):
mat(4, std::vector<int64_t>(length)),
matInt(4, std::vector<int64_t>(length)),
offsets(length),
minScoreColumn(length),
maxScoreColumn(length),
sum(length),
bestScore(length),
worstScore(length){
    this->length = length;
    granularity = 1.0;
    offset = 0;
    background[0] = pA;
    background[1] = pC;
    background[2] = pG;
    background[3] = pT;
}

void Matrix::toLogOddRatio () {
    for (int p = 0; p < length; p++) {
        double sum = mat[0][p] + mat[1][p] + mat[2][p] + mat[3][p];
        for (int k = 0; k < 4; k++) {
            mat[k][p] = log((mat[k][p] + 0.25) /(sum + 1)) - log (background[k]);
        }
    }
#ifdef PRINTLOGRATIO
    for (int k = 0; k < 4; k++ ) {
      for (int i = 0 ; i < length; i++) {
        cout << mat[k][i] << "\t";
      }
      cout << endl;
    }
#endif
}


// computes the complete score distribution between score min and max
void Matrix::showDistrib (int64_t min, int64_t max) {
    map<int64_t, double> *nbocc = calcDistribWithMapMinMax(min,max);
    map<int64_t, double>::iterator iter;

    // computes p values and stores them in nbocc[length]
    double sum = 0;
    map<int64_t, double>::reverse_iterator riter = nbocc[length-1].rbegin();
    while (riter != nbocc[length-1].rend()) {
        sum += riter->second;
        nbocc[length][riter->first] = sum;
        riter++;
    }

    iter = nbocc[length].begin();
    while (iter != nbocc[length].end() && iter->first <= max) {
        cout << (((iter->first)-offset)/granularity) << " " << (iter->second) << " " << nbocc[length-1][iter->first] << endl;
        iter ++;
    }
}


void Matrix::computesIntegerMatrix (double granularity, bool sortColumns) {
  double minS = 0, maxS = 0;
  double scoreRange;
  
  // computes precision
  for (int i = 0; i < length; i++) {
    double min = mat[0][i];
    double max = min;
    for (int k = 1; k < 4; k++ )  {
      min = ((min < mat[k][i])?min:(mat[k][i]));
      max = ((max > mat[k][i])?max:(mat[k][i]));
    }
    minS += min;
    maxS += max;
  } 
  
  // score range
  scoreRange = maxS - minS + 1;
  
  if (granularity > 1.0) {
    this->granularity = granularity / scoreRange;
  } else if (granularity < 1.0) {
    this->granularity = 1.0 / granularity;
  } else {
    this->granularity = 1.0;
  }
  for (int k = 0; k < 4; k++ ) {
    for (int p = 0 ; p < length; p++) {
      matInt[k][p] = std::round((double)(mat[k][p]*this->granularity));
    }
  }
  
#ifdef PRINTVERBOSE
  cout << "SCORE RANGE : " << minS << " -> " << maxS << endl;
  
  cout << "PRECISION " << this->granularity << endl;
  
  cout << "INTEGER MATRIX WITHOUT OFFSET" << endl;
  
  for (int k = 0; k < 4; k++ ) {
    for (int i = 0 ; i < length; i++) {
      cout << matInt[k][i] << "\t";
    }
    cout << endl;
  }
#endif
  
  this->errorMax = 0.0;
  for (int i = 1; i < length; i++) {
    double maxE = mat[0][i] * this->granularity - (matInt[0][i]);
    for (int k = 1; k < 4; k++) {
      maxE = ((maxE < mat[k][i] * this->granularity - matInt[k][i])?(mat[k][i] * this->granularity - (matInt[k][i])):(maxE));
    }
    this->errorMax += maxE;
  }
#ifdef PRINTVERBOSE
  cout << "  ERROR MAX : " << this->errorMax << endl;
#endif
  
  if (sortColumns) {
    // sort the columns : the first column is the one with the greatest value
    int64_t min = 0;
    for (int i = 0; i < length; i++) {
      for (int k = 0; k < 4; k++) {
        min = std::min(min,matInt[k][i]);
      }
    }
    min --;
    vector<int64_t> maxs(length);
    for (int i = 0; i < length; i++) {
      maxs[i] = matInt[0][i];
      for (int k = 1; k < 4; k++) {
        if (maxs[i] < matInt[k][i]) {
          maxs[i] = matInt[k][i];
        }
      }
    }

    std::vector<std::vector<int64_t>> mattemp(4, std::vector<int64_t>(length));

    for (int i = 0; i < length; i++) {
      int64_t max = maxs[0];
      int p = 0;
      for (int j = 1; j < length; j++) {
        if (max < maxs[j]) {
          max = maxs[j];
          p = j;
        }
      }
      maxs[p] = min;
      for (int k = 0; k < 4; k++) {
          mattemp[k][i]=matInt[k][p];
      }
    }
#ifdef PRINTVERBOSE
    cout << "INTEGER MATRIX WITHOUT OFFSET ORDERED" << endl;
    for (int k = 0; k < 4; k++)  {
      for (int i = 0; i < length; i++) {
        cout << mattemp[k][i] << "\t";
      }
      cout << endl;    
    }
#endif

    matInt=std::move(mattemp);

  }
  
  // computes offsets
  this->offset = 0;
  for (int i = 0; i < length; i++) {
    int64_t min = matInt[0][i];
    for (int k = 1; k < 4; k++ )  {
      min = ((min < matInt[k][i])?min:(matInt[k][i]));
    }
    offsets.push_back(-min);
    for (int k = 0; k < 4; k++ )  {
      matInt[k][i] += offsets[i];  
    }
    this->offset += offsets[i];
  }
  
#ifdef PRINTVERBOSE
  cout << "OFFSET : " << this->offset << endl;
#endif
  
#ifdef PRINTVERBOSE
  cout << "INTEGER MATRIX WITH OFFSET" << endl;
  for (int k = 0; k < 4; k++ )  {
    for (int i = 0; i < length; i++) {
      cout << matInt[k][i] << "\t";
    }
    cout << endl;    
  }
#endif
  
  
  // look for the minimum score of the matrix for each column
  for (int i = 0; i < length; i++) {
    minScoreColumn.push_back(matInt[0][i]);
    maxScoreColumn.push_back(matInt[0][i]);
    sum.push_back(0);
    for (int k = 1; k < 4; k++ )  {
      sum[i] = sum[i] + matInt[k][i];
      if (minScoreColumn[i] > matInt[k][i]) {
        minScoreColumn[i] = matInt[k][i];
      }
      if (maxScoreColumn[i] < matInt[k][i]) {
        maxScoreColumn[i] = matInt[k][i];
      }
    }
    minScore = minScore + minScoreColumn[i];
    maxScore = maxScore + maxScoreColumn[i];
    //cout << "minScoreColumn[" << i << "] = " << minScoreColumn[i] << endl;
    //cout << "maxScoreColumn[" << i << "] = " << maxScoreColumn[i] << endl;
  }
  this->scoreRange = maxScore - minScore + 1;
  
#ifdef PRINTVERBOSE
  cout << "SCORE RANGE : " << minScore << " - " << maxScore << " : " << this->scoreRange << endl;
#endif
  
  //bestScore.resize(length);
  //worstScore.resize(length);
  bestScore[length-1] = maxScore;
  worstScore[length-1] = minScore;
  for (int i = length - 2; i >= 0; i--) {
    bestScore[i]  = bestScore[i+1]  - maxScoreColumn[i+1];
    worstScore[i] = worstScore[i+1] - minScoreColumn[i+1];
  }
  
  
}




/**
* Computes the pvalue associated with the threshold score requestedScore.
 */
void Matrix::lookForPvalue (int64_t requestedScore, int64_t min, int64_t max, double *pmin, double *pmax) {
  
  map<int64_t, double> *nbocc = calcDistribWithMapMinMax(min,max); 
  map<int64_t, double>::iterator iter;
  
#ifdef SHOWCERR
  cerr << "  Looks for Pvalue between " << min << " and " << max << " for score " << requestedScore << endl;
#endif
  // computes p values and stores them in nbocc[length] 
  double sum = nbocc[length][max+1];
  int64_t s = max + 1;
  map<int64_t, double>::reverse_iterator riter = nbocc[length-1].rbegin();
  while (riter != nbocc[length-1].rend()) {
    sum += riter->second;
    if (riter->first >= requestedScore) s = riter->first;
    nbocc[length][riter->first] = sum;
    riter++;      
  }
  //cout << "   s found : " << s << endl;
  
  iter = nbocc[length].find(s);
  while (iter != nbocc[length].begin() && iter->first >= s - errorMax) {
    iter--;      
  }
  //cout << "   s - E found : " << iter->first << endl;
  
#ifdef MEMORYCOUNT
  // for tests, store the number of memory bloc necessary
  for (int pos = 0; pos <= length; pos++) {
    totalMapSize += nbocc[pos].size();
  }
#endif
  
  *pmax = nbocc[length][s];
  *pmin = iter->second;
  
  delete[] nbocc;
  
}



/**
* Computes the score associated with the pvalue requestedPvalue.
 */
int64_t Matrix::lookForScore (int64_t min, int64_t max, double requestedPvalue, double *rpv, double *rppv) {
  
  map<int64_t, double> *nbocc = calcDistribWithMapMinMax(min,max); 
  map<int64_t, double>::iterator iter;
#ifdef SHOWCERR
  cerr << "  Looks for score between " << min << " and " << max << endl;
#endif
  // computes p values and stores them in nbocc[length] 
  double sum = 0.0;
  map<int64_t, double>::reverse_iterator riter = nbocc[length-1].rbegin();
  int64_t alpha = riter->first+1;
  int64_t alpha_E = alpha;
  nbocc[length][alpha] = 0.0;
  while (riter != nbocc[length-1].rend()) {
    sum += riter->second;
    //cout << "Pv(S) " << riter->first << " " << sum << " " << requestedPvalue << endl;
    nbocc[length][riter->first] = sum;
    if (sum >= requestedPvalue) { 
      break;
    }
    riter++;      
  }
  //cout << "BREAK Pv(S) " << riter->first << " " << sum << " " << requestedPvalue << endl;
  if (sum > requestedPvalue) {
    alpha_E = riter->first;
    riter--;
    alpha = riter->first; 
  } else {
    if (riter == nbocc[length-1].rend()) { // path following the remark of the mail
      riter--;
      alpha = alpha_E = riter->first;
    } else {
      alpha = riter->first;
      riter++;
      sum += riter->second;
      alpha_E = riter->first;
    }
    nbocc[length][alpha_E] = sum;  
    //cout << "Pv(S) " << riter->first << " " << sum << endl;   
  }
#ifdef VERBOSE
  cerr << riter->first << "      ALPHA found at score " << alpha << " and P-value " << nbocc[length][alpha] << endl;
  cerr << riter->first << "      ALPHA-E found at score " << alpha_E << " and P-value " << nbocc[length][alpha_E] << endl;
#endif    
  
  // affichage des pvaleurs
  /*iter = nbocc[length].begin();
  while (iter != nbocc[length].end()) {
    cerr << iter->first << "[" << iter->second << "]" << endl;
    iter++;
  }*/
  
#ifdef MEMORYCOUNT
  // for tests, store the number of memory bloc necessary
  for (int pos = 0; pos <= length; pos++) {
    totalMapSize += nbocc[pos].size();
  }
#endif
  
  if (alpha - alpha_E > errorMax) alpha_E = alpha;
  
  *rpv = nbocc[length][alpha];
  *rppv = nbocc[length][alpha_E];   
  
  delete[] nbocc;
  return alpha;
  
}


// computes the distribution of scores between score min and max as the DP algrithm proceeds 
// but instead of using a table we use a map to avoid computations for scores that cannot be reached
map<int64_t, double> *Matrix::calcDistribWithMapMinMax (int64_t min, int64_t max) { 
  
  // maps for each step of the computation
  // nbocc[length] stores the pvalue
  // nbocc[pos] for pos < length stores the qvalue
  map<int64_t, double> *nbocc = new map<int64_t, double> [length+1];
  map<int64_t, double>::iterator iter;
  
  int64_t *maxs = new int64_t[length+1]; // @ pos i maximum score reachable with the suffix matrix from i to length-1
  
#ifdef VERBOSE    
  cerr << "  Calc distrib between " << min << " and " << max << endl;
#endif
  
  maxs[length] = 0;
  for (int i = length-1; i >= 0; i--) {
    maxs[i] = maxs[i+1] + maxScoreColumn[i];
  }
  
  // initializes the map at position 0
  for (int k = 0; k < 4; k++) {
    if (matInt[k][0]+maxs[1] >= min) {
      nbocc[0][matInt[k][0]] += background[k];
    }
  }
  
  // computes q values for scores greater or equal than min
  nbocc[length-1][max+1] = 0.0;
  for (int pos = 1; pos < length; pos++) {
    iter = nbocc[pos-1].begin();
    while (iter != nbocc[pos-1].end()) {
      for (int k = 0; k < 4; k++) {
        int64_t sc = iter->first + matInt[k][pos];
        if (sc+maxs[pos+1] >= min) {
          // the score min can be reached
          if (sc > max) {
            // the score will be greater than max for all suffixes
            nbocc[length-1][max+1] += nbocc[pos-1][iter->first] * background[k]; //pow(4,length-pos-1) ;
            totalOp++;
          } else {              
            nbocc[pos][sc] += nbocc[pos-1][iter->first] * background[k];
            totalOp++;
          }
        } 
      }
      iter++;      
    }      
    //cerr << "        map size for " << pos << " " << nbocc[pos].size() << endl;
  }
  
  
  delete[] maxs;
  
  return nbocc;
  
  
}




int64_t Matrix::fastPvalue (Matrix *m, int64_t alpha) {
  
  
  map<int64_t, int64_t> *q = new map<int64_t, int64_t> [m->length+1];
  map<int64_t, int64_t>::iterator iter;
  
  int64_t P = 0;
  
  int64_t *maxm = new int64_t[m->length+1]; // @ pos i maximum score reachable with the suffix matrix from i to length-1
  
  maxm[m->length] = 0;
  for (int i = m->length-1; i >= 0; i--) {
    maxm[i] = maxm[i+1] + m->maxScoreColumn[i];
  }
  
  // initializes the map at position 0
  for (int k = 0; k < 4; k++) {
    if (m->matInt[k][0]+maxm[1] >= alpha) {
      //cout << "FP: Set " << m->matInt[k][0] <<  " to ";
      q[0][m->matInt[k][0]] += 1;
      //cout << q[0][m->matInt[k][0]] << endl;        
    }
  }
  
  // computes q values for scores strictly greater than alpha
  for (int pos = 1; pos < m->length; pos++) {
    iter = q[pos-1].begin();
    while (iter != q[pos-1].end()) {
      for (int k = 0; k < 4; k++) {
        int64_t scm = iter->first + m->matInt[k][pos];
        if (scm > alpha) {
          //cout << "Update P from " << P;
          P += iter->second * (int64_t)pow(4.0,m->length-pos-1); 
          //cout << " to P " << P << endl;
        } else if (scm + maxm[pos+1] > alpha) {
          q[pos][scm] += iter->second; 
        }
      } 
      iter++;      
    }      
    q[pos-1].erase(q[pos-1].begin(),q[pos-1].end());
  }
  
  
  delete[] maxm;
  
  return P;
  
}
