#include <iostream>
#include <vector>
#include <algorithm>

void GaussSimplification(std::vector<std::vector<double> >& matrix, int32_t curRow) {
    for (int32_t row = curRow + 1; row < matrix.size(); ++row) {
        double factor = matrix[row][curRow] / matrix[curRow][curRow];
        for (int32_t i = 0; i < matrix[row].size(); ++i) {
            matrix[row][i] -= matrix[curRow][i] * factor;
        }
    }
    return;
}

int32_t FindMinRow(const std::vector<std::vector<double> >& matrix, const std::vector<int32_t>& cost,
                   int32_t from, int32_t to, int32_t column) {
    int32_t min = -1;
    int32_t minCost = 52;
    for (int32_t row = from; row < to; ++row) {
        if (matrix[row][column] != 0 && cost[row] < minCost) {
            minCost = cost[row];
            min = row;
        }
    }
    return min;
}

int32_t main() {
    int32_t m, n;
    std::cin >> m >> n;
    bool zeroesColumn = false;
    std::vector<int32_t> answer;
    std::vector<std::vector<double> > matrix(m);
    std::vector<int32_t> cost(m);
    std::vector<int32_t> number(m);
    for (int32_t i = 0; i < m; ++i) {
        matrix[i].resize(n);
    }
    for (int32_t row = 0; row < m; ++row) {
        for (int32_t col = 0; col < n; ++col) {
            std::cin >> matrix[row][col];
        }
        std::cin >> cost[row];
        number[row] = row + 1;
    }

    ////////
    int32_t minRow;
    for (int32_t curRow = 0; curRow < n; ++curRow) {
        minRow = FindMinRow(matrix, cost, curRow, m, curRow);
        if (minRow == -1) {
            zeroesColumn = true;
            break;
        }
        matrix[curRow].swap(matrix[minRow]);
        std::swap(cost[curRow], cost[minRow]);
        std::swap(number[curRow], number[minRow]);
        answer.push_back(number[curRow]);
        GaussSimplification(matrix, curRow);
    }

    ///answer
    std::sort(answer.begin(), answer.end());
    if (!zeroesColumn) {
        for (int32_t i = 0; i < answer.size() - 1; ++i) {
            std::cout << answer[i] << " ";
        }
        std::cout << answer[answer.size() - 1] << std::endl;
    } else {
        std::cout << "-1" << std::endl;
    }
    return 0;
}
