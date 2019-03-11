#include <iostream>
#include <string>
#include "TArbitraryPrecise.h"
int main(void) {

    std::string x, y;
    char action;

    while (std::cin >> x >> y >> action) {

        TArbitraryPrecise a(x);
        TArbitraryPrecise b(y);

        TArbitraryPrecise err(-1);

        TArbitraryPrecise result;

        bool compare = false;
        bool fin;

        switch (action) {
        case '+':
        {
            result = a + b;
            break;
        }
        case '-':
        {
            result = a - b;
            break;
        }
        case '*':
        {
            result = a * b;
            break;
        }
        case '/':
        {
            result = a / b;
            break;
        }
        case '^':
        {
            result = a ^ b;
            break;
        }
        case '>':
        {
            fin = ( a > b );
            compare = true;
            break;
        }
        case '<':
        {
            fin = ( a < b );
            compare = true;
            break;
        }
        case '=':
        {
            fin = ( a == b );
            compare = true;
            break;
        }
        }

        if (compare) {
            std::cout << (fin ? "true" : "false");
        } else if (result == err) {
            std::cout << "Error";
        } else {
            std::cout << result;
        }

        std::cout << std::endl;
    }

    return 0;
}
