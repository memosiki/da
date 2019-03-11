#include <iostream>
#include <string>
#include <cstdint>
int main() {
    int64_t m, answer, left, rigth;
    int64_t first, last;
    std::string n;

    std::cin >> n >> m;

    left = 1;
    rigth = 0;
    answer = 0;
    for (int32_t i = 0; i < n.length(); ++i) {
        //scope [left ; rigth]
        //from first to last
        if (i != 0) {
            rigth *= 10;
            left *= 10 ;
        }
        rigth += n[i] - '0';
        //rigth rounds to floor
        //left to ceil
        if (left % m)
            first = left  + m - left % m;
        else
            first = left;
        last = rigth - rigth % m;

        if (first <= last)
            answer += (last - first) / m + 1;
        //else there is no such number in this scope
    }
    if (rigth % m == 0)
        //very n does not count
        answer--;
    std::cout << answer << std::endl;
    return 0;
}