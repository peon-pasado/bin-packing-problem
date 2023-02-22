#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

int main() {
    int n, m; cin >> n >> m;

    vector<vector<int>> adj = vector<vector<int>>(n, vector<int>());
    vector<pair<int, int>> edge_list;

    for(int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a -= 1;
        b -= 1;
        adj[a].push_back(b);
        adj[b].push_back(a);
        edge_list.push_back(make_pair(min(a, b), max(a, b)));
    }

    vector<pair<int, int>> removed_edges;
    for(auto p : edge_list) {
        vector<bool> seen = vector<bool>(n, false);
        seen[0] = true;
        stack<int> s;
        s.push(0);
        
        while(!s.empty()) {
            int cur = s.top(); s.pop();
            for(int nb : adj[cur]) {
                if(make_pair(min(cur, nb), max(cur, nb)) != p) {
                    if(!seen[nb]) {
                        s.push(nb);
                        seen[nb] = true;
                    }
                }
            }
        }
        bool connected = true;
        for(bool a : seen)
            connected &= a;

        if(connected) {
            removed_edges.push_back(p);
            int a = p.first;
            int b = p.second;

            auto pos = find(adj[a].begin(), adj[a].end(), b);
            adj[a].erase(pos);

            pos = find(adj[b].begin(), adj[b].end(), a);
            adj[b].erase(pos);
        }
    }

    int root = 0;
    vector<int> num_outgoing = vector<int>(n);
    for(int i = 0; i < n; i++) {
        num_outgoing[i] = ((int)adj[i].size()) - 1;
    }
    num_outgoing[root] += 1;
    vector<int> toposort;
    stack<int> s;
    for(int i = 0; i < n; i++) {
        if(num_outgoing[i] == 0) {
            toposort.push_back(i);
            s.push(i);
        }
    }
    vector<int> parent(n, -1);
    while(!s.empty()) {
        int cur = s.top(); s.pop();
        for(int nb : adj[cur]) {
            if(num_outgoing[nb] != 0) {
                parent[cur] = nb;
                num_outgoing[nb] -= 1;
                if(num_outgoing[nb] == 0) {
                    s.push(nb);
                    toposort.push_back(nb);
                }
            }
        }
    }

    int largest = 0;
    m = removed_edges.size();

    for(int mask = 0; mask < 1 << m; mask++) {
        vector<int> off_verts;
        for(int j = 0; j < m; j++)
            off_verts.push_back(
                    (mask & (1 << j)) ? 
                    removed_edges[j].first :
                    removed_edges[j].second);

        vector<bool> is_off(n, false);
        for(int a : off_verts)
            is_off[a] = true;

        /*printf("Mask: %d\n", mask);
        printf("Off_verts: %d\n", off_verts.size());
        for(int ov : off_verts)
            printf("%d\n", ov);*/

        vector<int> max_ind_used(n, 0);
        vector<int> max_ind_unused(n, 0);

        for(int x : toposort) {
            for(int nb : adj[x]) {
                if(nb != parent[x]) {
                    max_ind_unused[x] += max(max_ind_used[nb], max_ind_unused[nb]);
                }
            }

            if(!is_off[x]) {
                max_ind_used[x] = 1;
                for(int nb : adj[x]) {
                    if(nb != parent[x]) {
                        max_ind_used[x] += max_ind_unused[nb];
                    }
                }
            }
            else {
                max_ind_used[x] = max_ind_unused[x];
            }
        }
        /*printf("max_ind_used, max_ind_unused\n");
        for(int v = 0; v < n; v++)
            printf("%d %d %d\n", v, max_ind_used[v], max_ind_unused[v]);*/
        largest = max(largest,
                max(max_ind_used[0], max_ind_unused[0]));
        /*printf("largest: %d\n", largest);
        printf("\n");*/
    }

    cout << largest << endl;

    return 0;
}

/*

largest = 0

for l in itertools.product([0, 1], repeat=len(removed_edges)):
    off_verts = [p[i] for p, i in zip(removed_edges, l)]

    max_ind_used = [0]*n
    max_ind_unused = [0]*n

    for x in toposort:
        for nb in adj[x]:
            if nb != parent[x]:
                max_ind_unused[x] += max_ind_used[nb]

        if x not in off_verts:
            max_ind_used[x] = 1
            for nb in adj[x]:
                if nb != parent[x]:
                    max_ind_used[x] += max_ind_unused[nb]
        else:
            max_ind_used[x] = max_ind_unused[x]
    largest = max(largest, max_ind_used[0], max_ind_unused[0])

print(largest)
    */
