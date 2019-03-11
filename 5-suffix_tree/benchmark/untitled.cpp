#include <string>
#include <iostream>

int main(void)
{
    std::string text;
    std::string pattern;
    std::cin >> text;
    int count = 1; 
    size_t found;
    std::vector<int> res;
    while (std::cin >> pattern){
        found = text.find(pattern);
        while(found!=std::string::npos){
            res.push_back(found+1);
            found = text.find(pattern, found+1);
        }
        if (!res.empty()) {
            std::cout << count << ": ";
            for (int i = 0; i < res.size()-1; ++i)
                std::cout << res[i] << ", ";
            std::cout << res[res.size()-1] << std::endl;
        }
        ++count;
    }
    return 0;
}