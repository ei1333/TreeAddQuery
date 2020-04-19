#include "testlib.h"
#include<bits/stdc++.h>

using namespace std;

template< typename sum_t, typename key_t >
struct ReRooting {
  struct Edge {
    int to;
    key_t data;
    sum_t dp, ndp;
  };

  using F = function< sum_t(sum_t, sum_t) >;
  using G = function< sum_t(sum_t, key_t) >;

  vector< vector< Edge > > g;
  vector< sum_t > subdp, dp;
  const sum_t ident;
  const F f;
  const G gg;

  ReRooting(int V, const F f, const G g, const sum_t &ident)
      : g(V), f(f), gg(g), ident(ident), subdp(V, ident), dp(V, ident) {}

  void add_edge(int u, int v, const key_t &d) {
    g[u].emplace_back((Edge) {v, d, ident, ident});
    g[v].emplace_back((Edge) {u, d, ident, ident});
  }

  void add_edge_bi(int u, int v, const key_t &d, const key_t &e) {
    g[u].emplace_back((Edge) {v, d, ident, ident});
    g[v].emplace_back((Edge) {u, e, ident, ident});
  }

  void dfs_sub(int idx, int par) {
    for(auto &e : g[idx]) {
      if(e.to == par) continue;
      dfs_sub(e.to, idx);
      subdp[idx] = f(subdp[idx], gg(subdp[e.to], e.data));
    }
  }

  void dfs_all(int idx, int par, const sum_t &top) {
    sum_t buff{ident};
    for(int i = 0; i < (int) g[idx].size(); i++) {
      auto &e = g[idx][i];
      e.ndp = buff;
      e.dp = gg(par == e.to ? top : subdp[e.to], e.data);
      buff = f(buff, e.dp);
    }
    dp[idx] = buff;
    buff = ident;
    for(int i = (int) g[idx].size() - 1; i >= 0; i--) {
      auto &e = g[idx][i];
      if(e.to != par) dfs_all(e.to, idx, f(e.ndp, buff));
      e.ndp = f(e.ndp, buff);
      buff = f(buff, e.dp);
    }
  }

  vector< sum_t > build() {
    dfs_sub(0, -1);
    dfs_all(0, -1, ident);
    return dp;
  }
};


// 制約
const int N_MIN = 1;
const int N_MAX = 3000;
const int Q_MIN = 1;
const int Q_MAX = 3000;
const int A_MIN = 1;
const int A_MAX = 1000000000;


int main(int argc, char *argv[])
{
  registerGen(argc, argv, 1);
  for (int t = 0; t < 5; t++) {
    ofstream of(format("02_small_%02d.in", t+1).c_str());

    int N = rnd.next(N_MAX - 1000, N_MAX);
    int Q = rnd.next(Q_MAX - 1000, Q_MAX);
    vector<int> p(N);
    for(int i = 1; i < N; i++) p[i] = rnd.wnext(i, t * 10 + 1);

    vector<int> perm(N);
    iota(begin(perm), end(perm), 0);
    shuffle(perm.begin() + 1, perm.end());

    vector<pair<int,int> > edges;
    for (int i = 1; i < N; i++)
        if (rnd.next(2)) edges.push_back(make_pair(perm[i], perm[p[i]]));
        else edges.push_back(make_pair(perm[p[i]], perm[i]));

    shuffle(edges.begin(), edges.end());

    of << N << " " << Q << endl;
    auto f = [](int a, int b) { return max(a, b); };
    auto g2 = [](int a, int b) { return a + b; };
    ReRooting< int, int > g(N, f, g2, 0); 
    for (int i = 0; i + 1 < N; i++) {
        of << edges[i].first + 1 << " " << edges[i].second + 1 << endl;
        g.add_edge(edges[i].first, edges[i].second, 1);
    }
    auto dp = g.build();
    for(int i = 0; i < Q; i++) {
      int X = rnd.next(1, N);
      int Y = rnd.next(0, dp[X - 1]);
      int Z = rnd.next(A_MIN, A_MAX);
      of << X << " " << Y << " " << Z << endl;
    } 

    of.close();
  }
  return 0;
}

