#include <cstdlib>
#include <iostream>
#include <cmath>
#include "TVector.h"

TVector::TVector(size_t initialSize) {
    array = (TKey*) malloc(initialSize * sizeof(TKey));
    size = 0;
    capacity = initialSize;
}

void TVector::Insert(TKey element) {
    if (capacity == size) {
        capacity *= 2;
        array = (TKey *) realloc(array, capacity * sizeof(TKey));
    }
    array[size] = element;
    size++;
}
void TVector::Print() {
    for (size_t var = 0; var < size; ++var) {
        std::cout << array[var].value << '\t';
        array[var].link->Print();
        std::cout << std::endl;
    }
}
TKey* TVector::First(){
    if (size==0)
        return NULL;
    return &array[0];
}

TKey* TVector::Last(){
    if (size==0)
        return NULL;
    return &array[size];

}

bool TVector::IsSorted() {
    for (size_t i = 1; i < size; ++i) {
        if (array[i - 1].value > array[i].value) {
            return false;
        }
    }
    return true;
}
void TVector::RadixSort() {
    //array of 0 or 1 item do not need sorting
    if (size <= 1)
        return;
    
    size_t bits = 0;
    TKey* B = (TKey*) malloc(this->size * sizeof(TKey));
    //copy of vector
    for (size_t i = 0; i < size; ++i) {
        B[i] = this->array[i];
    }

    //choosing parameter of sorting
    if (sizeof(unsigned long long) * 8 < floor(log2(size))) {
        bits = sizeof(unsigned long long) * 8;
    }
    else {
        bits = floor(log2(size));
    }
    //bit length of key
    size_t length = sizeof(unsigned long long) * 8;

    //max number that can be coded with this number of bits
    unsigned long long k = pow(2, bits);

    //array for counting
    unsigned long long *C = (unsigned long long*) malloc(
            k * sizeof(unsigned long long));
    unsigned long long index;
    unsigned long long shifted;
    unsigned long long carry;
    unsigned long long temp;
    for (size_t shift = 0; shift < ceil(length * 1.0 / bits); ++shift) {
        for (index = 0; index < k; ++index) {
            C[index] = 0;
        }
        for (index = 0; index < size; ++index) {
            C[getBits(B[index].value, shift, bits)]++;
        }
        carry = 0;
        for (index = 0; index < k; ++index) {
            temp = C[index];
            C[index] = carry;
            carry += temp;
        }
        for (index = 0; index < size; ++index) {
            shifted = getBits(B[index].value, shift, bits);
            //if there is "count" elements lower or equal than current => his position count-1
            this->array[C[shifted]] = B[index];
            C[shifted]++;
        }
        for (size_t i = 0; i < size; ++i) {
            B[i] = this->array[i];
        }
    }
    delete[] C;
    delete[] B;
}
unsigned long long TVector::getBits(unsigned long long number, size_t shift,
        size_t bits) {

    size_t length = sizeof(number) * 8;
    number = number >> shift * bits;
    number = number << (length - bits);
    number = number >> (length - bits);
    return number;
}
TVector::~TVector() {
    for (size_t i = 0; i < size; ++i) {
        delete array[i].link;
    }
    delete[] array;
}
