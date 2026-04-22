//
// Created by belinard on 22/04/26.
//

#ifndef TFM_PVALUE_PARSERS_H
#define TFM_PVALUE_PARSERS_H


#include "Matrix.h"

class Parsers {

private:

    /**
    * Split a string following delimiters
     */
    static void tokenize(const string& str, vector<string>& tokens, const string& delimiters);

public:

    static Matrix readJasparMatrix (const string& filename);
    static Matrix readHorizontalMatrix (const string& filename);

};


#endif //TFM_PVALUE_PARSERS_H
