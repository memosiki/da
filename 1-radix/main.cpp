#include <iostream>
#include <cstdlib>
#include "TString.h"
#include "TVector.h"

int main(int argc, char** argv) {
    //init variablesC
    size_t defaultVectorSize = 2;
    TVector* vect = new TVector(defaultVectorSize);
    TKey key;
    TString *str;
    
    //if input file is empty it will trigger iostate flags
    std::cin >> key.value;
    while (!std::cin.fail() && !std::cin.eof()) {
		std::cin.get();//getting tab
        str = new TString(std::cin);
        key.link = str;
        vect->Insert(key);
        std::cin >> key.value;
    }

    vect->RadixSort();
    vect->Print();

    delete vect;
    return 0;
}
