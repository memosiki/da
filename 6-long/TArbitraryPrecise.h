#ifndef TARBITRARYPRECISE_H
#define TARBITRARYPRECISE_H

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

class TArbitraryPrecise {
public:
    TArbitraryPrecise(std::string  num);
    TArbitraryPrecise(const std::vector <int32_t>& num);
    TArbitraryPrecise(const int32_t num);
    TArbitraryPrecise() {};
    ~TArbitraryPrecise() {};
    bool Even() const;

    friend std::ostream& operator<<(std::ostream& os, const TArbitraryPrecise &b);
    bool operator ==(const TArbitraryPrecise &x) const;
    bool operator > (const TArbitraryPrecise &x) const;
    bool operator < (const TArbitraryPrecise &x) const;
    TArbitraryPrecise operator + (const TArbitraryPrecise &b) const;
    TArbitraryPrecise operator - (const TArbitraryPrecise &b) const;
    TArbitraryPrecise operator * (const TArbitraryPrecise &b) const;
    TArbitraryPrecise operator / (const TArbitraryPrecise &b) const;
    TArbitraryPrecise operator ^ (const TArbitraryPrecise &b) const;
private:
    int32_t CompareDigits(const std::vector<int32_t> &) const;
    void RemoveZeroes();
    void FillZeroes(int32_t count);
    static const int32_t base = 10000;
    std::vector<int32_t> digit;


};

#endif
