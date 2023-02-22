#include <bits/stdc++.h>
using namespace std;

vector<bool> bt(int lim) {
    //x - a^3 = (c + d) * (c^2 - cd + d^2)
    //y = c^2 - cd + d^2
    //0 = X^2 - Xd + (d^2 - y)
    //X = (d +- sqrt(d^2 - 4 * (d^2 - y))) / 2
    //X = (d +- sqrt(4y - 3*d^2))/2
    //4y - 3d^2 = s^2
    //

	set<long long> s;
	for (int i = 0; i <= lim; ++i) {
		for (int j = i; j <= lim; ++j) {
			long long t = 1ll*i*i*i + 1ll*j*j*j;
			long long u = 1ll*i*i*i - 1ll*j*j*j;
			s.insert(t);
			s.insert(-t);
			s.insert(u);
			s.insert(-u);
		}
	}
	vector<bool> vis(201);
	for (int x = 0; x <= 200; ++x) {
		for (int i = -lim; i <= lim; ++i) {
			if (s.count(x + 1ll*i*i*i)) {
				vis[x] = 1;
				break;
			}
		}
	}
	return vis;
}

int main() {
	assert(bt(1000) == bt(4000));
    cout << "good" << '\n';
	return 0;
}