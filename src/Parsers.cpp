//
// Created by belinard on 22/04/26.
//

#include "../include/Matrix.h"
#include "../include/Parsers.h"

void Parsers::tokenize(const string& str, vector<string>& tokens, const string& delimiters) {

    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}


Matrix Parsers::readJasparMatrix (const string& filename) {

    ifstream f(filename.data());
    if (!f) {
        throw new FileException();
    }

    string str;
    vector<string> v;

    // init matrix from 1st line
    getline(f,str);
    tokenize(str,v," \t|");
    auto m = Matrix(v.size());
    for (unsigned int i = 0; i < v.size(); i++) {
        m.mat[0][i] = atof(v.at(i).data());
    }
    v.clear();

    //do other lines
    for (int j = 1; j < 4; j++) {
        getline(f,str);
        tokenize(str,v," \t|");
        for (unsigned int i = 0; i < v.size(); i++) {
            m.mat[j][i] = atof(v.at(i).data());
        }
        v.clear();
    }

    f.close();


#ifdef PRINTVERBOSE
    cout << "INITIAL MATRIX" << endl;
    for (int k = 0; k < 4; k++ ) {
      for (int i = 0 ; i < length; i++) {
        cout << m.mat[k][i] << "\t";
      }
      cout << endl;
    }
#endif

    return m;
}


Matrix Parsers::readHorizontalMatrix (const string& filename) {

    ifstream f(filename.data());
    if (!f) {
        throw new FileException();
    }

    string str;
    // comment out for JASPAR matrices
    getline(f,str); // line with matrix name and family
    vector<string> v;

    //1st line to get length
    getline(f,str);
    tokenize(str,v," \t|");
    auto m = Matrix(v.size() -1); // not -1 for JASPAR
    for (unsigned int i = 1; i < v.size(); i++) { // 1 if not JASPAR
        m.mat[0][i-1] = atof(v.at(i).data());
    }
    v.clear();

    for (int j = 1; j < 4; j++) {
        getline(f,str);
        tokenize(str,v," \t|");
        for (unsigned int i = 1; i < v.size(); i++) { // 1 if not JASPAR
            m.mat[j][i-1] = atof(v.at(i).data());
        }
        v.clear();
    }

    f.close();

    return m;

}