#include <iostream>
#include <algorithm>
#include <string>
#include "TVector.h"

bool Compare(const TKey arg1, const TKey arg2) {
    return (arg1.value < arg2.value);
	}

void RunSort(TVector* vec, std::string s) {
    time_t start = clock();
    if (s=="radix") {
        vec->RadixSort();
    }
    else if (s=="std::stable_sort") {
        std::stable_sort(vec->First(), vec->Last(), Compare);
    }
    else {
        std::cerr << "Unknown type of sorting" << std::endl;
        return;
    }
    time_t end = clock();
    if (!vec->IsSorted()) {
        std::cout << "Sorting " << s << " failed!" << std::endl << std::endl;
    }
    else {
        std::cout << "Sorting: " << s << std::endl;
        std::cout << "Time of working "
                << ((double) (end - start) / (double) CLOCKS_PER_SEC) * 1000
                << "ms." << std::endl << std::endl;
    }
}

int main(int argc, const char *argv[]) {
    size_t defaultVectorSize = 2;
    TVector* vect1 = new TVector(defaultVectorSize);
    TVector* vect2 = new TVector(defaultVectorSize);
    TKey key;
    TString *str;
    std::cin >> key.value;
    while (!std::cin.fail() && !std::cin.eof()) {
        std::cin.get(); //reading tab
        str = new TString(std::cin);
        key.link = str;
        vect1->Insert(key);
        vect2->Insert(key);
        std::cin >> key.value;
    }
    RunSort(vect1, "radix");
    RunSort(vect2, "std::stable_sort");

    return 0;
}
