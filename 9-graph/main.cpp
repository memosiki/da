#include <iostream>
#include <vector>
#include <algorithm>
#include <vector>
#include <queue>
void Dfs(int start, std::vector<bool> &visited, const std::vector<std::vector<int> > &adj,
         std::vector<int> &component) {
    visited[start] = true;
    component.push_back(start);
    for (int node : adj[start]) {
        if (!visited[node])
            Dfs(node, visited, adj, component);
    }
}
int main() {
    int n = 0, m = 0;
    std::cin >> n >> m;
    std::vector<std::vector<int>> adj;
    std::vector<std::vector<int>> answer;
    std::vector<int> component;
    adj.resize(n);
    std::vector<bool> visited;
    visited.resize(n);
    for (int i = 0; i < n; ++i)
        visited[i] = false;
    int index, node;

    //input
    while (std::cin >> index) {
        std::cin >> node;
        adj[index - 1].push_back(node - 1);
        adj[node - 1].push_back(index - 1);
    }

    //dfs
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            Dfs(i, visited, adj, component);
            std::sort(component.begin(), component.end());
            answer.push_back(component);
            component.clear();
        }
    }
    //sorting answers
    std::sort(answer.begin(), answer.end(),
              [] (std::vector<int> const & a, std::vector<int> const & b)
    { return a[0] < b[0]; });

    for (int i = 0; i < answer.size(); ++i) {
        for (int j = 0; j < answer[i].size() - 1; ++j) {
            std::cout << answer[i][j] + 1 << " ";
        }
        std::cout << answer[i][answer[i].size() - 1]+1;
        std::cout << std::endl;
    }
}
