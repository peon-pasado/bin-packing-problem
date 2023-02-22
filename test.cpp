#include <bits/stdc++.h>
using namespace std;

int main() {
    int n = 30;
    cout << n << '\n';
    vector<vector<int>> A(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            A[i][j] = A[j][i] = abs(rand());
        }
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << A[i][j] << ' ';
        }
        cout << '\n';
    }
    return 0;
}