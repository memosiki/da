#ifndef TSTRING_H
#define TSTRING_H
#include <iostream>

class TString {
public:
    TString();
    TString(std::istream &is);
    TString(char in[], size_t);
    void Print();
    virtual ~TString();
private:
    static const size_t MAXSIZE = 65;
    char arr[MAXSIZE];
};

#endif /* TSTRING_H */
