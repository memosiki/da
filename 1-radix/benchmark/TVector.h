#ifndef TVECTOR_H
#define TVECTOR_H
#include <iostream>
#include "TString.h"

struct TKey {
    unsigned long long value;
    TString* link;
};

class TVector {
public:
    TVector(size_t initialSize);
    void Insert(TKey element);
    void Print();
    TKey* First();
    TKey* Last();
    bool IsSorted();
    void RadixSort();
    ~TVector();

private:
    unsigned long long getBits(unsigned long long number, size_t shift,
            size_t bits);
    TKey *array;
    size_t size;
    size_t capacity;
};

#endif /* TVECTOR_H */
