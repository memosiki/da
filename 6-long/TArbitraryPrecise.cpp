#include "TArbitraryPrecise.h"


TArbitraryPrecise::TArbitraryPrecise(std::string  num) {
    int32_t baseSize = std::to_string(base).length() - 1;

    for (int32_t i = num.length(); i > 0; i -= baseSize) {
        if (i < baseSize)
            digit.push_back(std::stoi(num.substr(0, i)));
        else
            digit.push_back(std::stoi(num.substr(i - baseSize, baseSize)));
    }
    RemoveZeroes();
}

TArbitraryPrecise::TArbitraryPrecise(const std::vector <int32_t> &num): digit(num) {
    RemoveZeroes();
}

TArbitraryPrecise::TArbitraryPrecise(const int32_t num) {
    digit.push_back(num);
}

void TArbitraryPrecise::RemoveZeroes() {
    while (digit.back() == 0 && digit.size() > 1)
        digit.pop_back();
}
void TArbitraryPrecise::FillZeroes(int32_t count) {
    for (int32_t i = 0; i < count; i++)
        digit.push_back(0);
}

int32_t TArbitraryPrecise::CompareDigits(const std::vector<int32_t> &b) const {
    int32_t aLen = this->digit.size();
    int32_t bLen = b.size();

    if (aLen < bLen)
        return -1;
    if (aLen > bLen)
        return  1;

    for (int32_t i = aLen - 1; i >= 0; i--) {
        if (this->digit[i] > b[i])
            return  1;
        if (this->digit[i] < b[i])
            return -1;
    }
    return 0;
}

bool TArbitraryPrecise::operator == (const TArbitraryPrecise &b) const {
    return CompareDigits(b.digit) == 0;
}

bool TArbitraryPrecise::operator > (const TArbitraryPrecise &b) const {
    return CompareDigits(b.digit) > 0;
}

bool TArbitraryPrecise::operator < (const TArbitraryPrecise &b) const {
    return CompareDigits(b.digit) < 0;
}

TArbitraryPrecise TArbitraryPrecise::operator + (const TArbitraryPrecise &b) const {
    std::vector<int32_t> result;
    int32_t aLen = this->digit.size();
    int32_t bLen = b.digit.size();
    int32_t remainder = 0;
    int32_t maxLen = std::max(aLen, bLen);
    int32_t place;

    for (int32_t i = 0; (i < maxLen); i++) {
        place = (i >= aLen ? 0 : this->digit[i]) + (i >= bLen ? 0 : b.digit[i]) + remainder;
        if (place >= base) {
            remainder = 1;
            result.push_back(place - base);
        } else {
            result.push_back(place);
            remainder = 0;
        }
    }

    if (remainder) {
        result.push_back(remainder);
    }
    TArbitraryPrecise res(result);
    res.RemoveZeroes();
    return res;
}

TArbitraryPrecise TArbitraryPrecise::operator - (const TArbitraryPrecise &b) const {
    std::vector<int32_t> result;

    if (*this < b) {
        result.push_back(-1);
    } else if (*this == b) {
        result.push_back(0);
    } else {
        int32_t aLen = this->digit.size();
        int32_t bLen = b.digit.size();
        int32_t remainder = 0;
        int32_t place;

        for (int32_t i = 0; i < aLen; i++) {
            place = this->digit[i] - (i >= bLen ? 0 : b.digit[i]) - remainder;
            if (place < 0) {
                remainder = 1;
                result.push_back(place + base);
            } else {
                remainder = 0;
                result.push_back(place);
            }
        }
    }
    TArbitraryPrecise x(result);
    x.RemoveZeroes();
    return x;
}

TArbitraryPrecise TArbitraryPrecise::operator * (const TArbitraryPrecise &b) const {
    if (*this == 0 || b == 0)
        return TArbitraryPrecise(0);

    int32_t aLen = this->digit.size();
    int32_t bLen = b.digit.size();
    std::vector <int32_t> result(aLen + bLen, 0);
    int32_t remainder;

    for (int32_t i = 0; i < aLen; i++) {

        remainder = 0;
        for (int32_t j = 0; j < bLen; j++) {
            int64_t part = result[i + j] + (this->digit[i] *  b.digit[j]) + remainder;

            result[i + j] = (int32_t )(part % base);
            remainder =  (int32_t )(part / base);
        }
        result[i + bLen] += remainder;
    }
    TArbitraryPrecise x(result);
    x.RemoveZeroes();
    return x;
}


bool TArbitraryPrecise::Even() const {
    if (digit[0] % 2 == 0)
        return true;
    else
        return false;
}

TArbitraryPrecise TArbitraryPrecise::operator / (const TArbitraryPrecise &b) const {
    int32_t aLen  = this->digit.size();
    TArbitraryPrecise current(0);

    //dividing by zero
    if (b == current)
        return TArbitraryPrecise(-1);

    if (*this < b)
        return  current;

    TArbitraryPrecise check;
    TArbitraryPrecise result;
    result.FillZeroes(aLen);

    int32_t place, left, right, middle;

    for (int32_t i = aLen - 1; i >= 0; i--) {
        //add one digit at the beginning
        current.digit.insert(current.digit.begin(), this->digit[i]);
        current.RemoveZeroes();
        place = 0;
        left = 0;
        right = base;
        //binary search
        if (current < b) {
            continue;
        }
        while (left <= right) {
            middle = (left + right) / 2;
            check = b * TArbitraryPrecise(middle);
            if (!(check > current)) {
                place = middle;
                left = middle + 1;
            } else {
                right = middle - 1;
            }
        }
        result.digit[i] = place;
        current = current - (b * TArbitraryPrecise(place));
    }
    result.RemoveZeroes();
    return result;
}


TArbitraryPrecise TArbitraryPrecise::operator ^ (const TArbitraryPrecise &b) const {
    TArbitraryPrecise zero(0);
    TArbitraryPrecise one(1);
    if ((*this) == zero ) {
        if (b == zero)
            return TArbitraryPrecise(-1);
        return zero;
    }
    if (b == zero)
        return one;

    if (b == one || *this == one)
        return *this;

    if ( b.Even()) {
        TArbitraryPrecise result = (*this) ^ (b / TArbitraryPrecise(2));
        return result * result;
    }
    return (*this) * ((* this) ^ (b - one));
}


std::ostream& operator << (std::ostream &os, const TArbitraryPrecise &b) {

    int32_t len = b.digit.size();
    int32_t baseSize = std::to_string(b.base).length() - 1;

    //first symbol without leading zeroes
    if (len)
        os << b.digit[len - 1];
    for (int32_t i = len - 2; i >= 0; i--) {
        os << std::setw(baseSize) << std::setfill('0') << b.digit[i];
    }
    return os;
}