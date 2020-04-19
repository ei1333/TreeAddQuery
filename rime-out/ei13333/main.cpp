#include <bits/stdc++.h>

using namespace std;

using int64 = long long;
const int mod = 1e9 + 7;

const int64 infll = (1LL << 58) - 1;
const int inf = (1 << 30) - 1;

struct IoSetup {
  IoSetup() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(10);
    cerr << fixed << setprecision(10);
  }
} iosetup;


template< typename T1, typename T2 >
ostream &operator<<(ostream &os, const pair< T1, T2 > &p) {
  os << p.first << " " << p.second;
  return os;
}

template< typename T1, typename T2 >
istream &operator>>(istream &is, pair< T1, T2 > &p) {
  is >> p.first >> p.second;
  return is;
}

template< typename T >
ostream &operator<<(ostream &os, const vector< T > &v) {
  for(int i = 0; i < (int) v.size(); i++) {
    os << v[i] << (i + 1 != v.size() ? " " : "");
  }
  return os;
}

template< typename T >
istream &operator>>(istream &is, vector< T > &v) {
  for(T &in : v) is >> in;
  return is;
}

template< typename T1, typename T2 >
inline bool chmax(T1 &a, T2 b) { return a < b && (a = b, true); }

template< typename T1, typename T2 >
inline bool chmin(T1 &a, T2 b) { return a > b && (a = b, true); }

template< typename T = int64 >
vector< T > make_v(size_t a) {
  return vector< T >(a);
}

template< typename T, typename... Ts >
auto make_v(size_t a, Ts... ts) {
  return vector< decltype(make_v< T >(ts...)) >(a, make_v< T >(ts...));
}

template< typename T, typename V >
typename enable_if< is_class< T >::value == 0 >::type fill_v(T &t, const V &v) {
  t = v;
}

template< typename T, typename V >
typename enable_if< is_class< T >::value != 0 >::type fill_v(T &t, const V &v) {
  for(auto &e : t) fill_v(e, v);
}

template< typename F >
struct FixPoint : F {
  FixPoint(F &&f) : F(forward< F >(f)) {}

  template< typename... Args >
  decltype(auto) operator()(Args &&... args) const {
    return F::operator()(*this, forward< Args >(args)...);
  }
};

template< typename F >
inline decltype(auto) MFP(F &&f) {
  return FixPoint< F >{forward< F >(f)};
}

template< typename T >
struct edge {
  int src, to;
  T cost;

  edge(int to, T cost) : src(-1), to(to), cost(cost) {}

  edge(int src, int to, T cost) : src(src), to(to), cost(cost) {}

  edge &operator=(const int &x) {
    to = x;
    return *this;
  }

  operator int() const { return to; }
};

template< typename T >
using Edges = vector< edge< T > >;
template< typename T >
using WeightedGraph = vector< Edges< T > >;
using UnWeightedGraph = vector< vector< int > >;
template< typename T >
using Matrix = vector< vector< T > >;

template< class T >
struct BinaryIndexedTree {
  vector< T > data;

  BinaryIndexedTree() = default;

  BinaryIndexedTree(int sz) {
    data.assign(++sz, 0);
  }

  T sum(int k) {
    T ret = 0;
    for(++k; k > 0; k -= k & -k) ret += data[k];
    return (ret);
  }

  void add(int k, T x) {
    for(++k; k < data.size(); k += k & -k) data[k] += x;
  }
};

template< typename G >
struct CentroidDecomposition {
  const G &g;
  vector< int > sub;
  vector< bool > v;

  CentroidDecomposition(const G &g) : g(g), sub(g.size()), v(g.size()) {}

  inline int build_dfs(int idx, int par) {
    sub[idx] = 1;
    for(auto &to : g[idx]) {
      if(to == par || v[to]) continue;
      sub[idx] += build_dfs(to, idx);
    }
    return sub[idx];
  }

  inline int search_centroid(int idx, int par, const int mid) {
    for(auto &to : g[idx]) {
      if(to == par || v[to]) continue;
      if(sub[to] > mid) return search_centroid(to, idx, mid);
    }
    return idx;
  }


  inline int build(UnWeightedGraph &t, int idx) {
    int centroid = search_centroid(idx, -1, build_dfs(idx, -1) / 2);
    v[centroid] = true;
    for(auto &to : g[centroid]) {
      if(!v[to]) t[centroid].emplace_back(build(t, to));
    }
    v[centroid] = false;
    return centroid;
  }

  inline int build(UnWeightedGraph &t) {
    t.resize(g.size());
    return build(t, 0);
  }
};

int main() {
  int N, Q;
  cin >> N >> Q;
  UnWeightedGraph g(N);
  for(int i = 1; i < N; i++) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    g[a].emplace_back(b);
    g[b].emplace_back(a);
  }
  CentroidDecomposition< UnWeightedGraph > cd(g);
  UnWeightedGraph t;
  vector< int > used(N);

  vector< int > depth(N);
  vector< vector< int > > subdepth(N);

  vector< vector< tuple< int, int, int > > > belong(N);
  vector< BinaryIndexedTree< int64 > > bit(N);
  vector< vector< BinaryIndexedTree< int64 > > > subbit(N);

  MFP([&](auto rec, int centroid) -> void {
    used[centroid] = true;

    queue< tuple< int, int, int, int > > que;
    int par_id = 0;
    belong[centroid].emplace_back(centroid, 0, -1);
    for(auto &to : g[centroid]) {
      if(!used[to]) que.emplace(to, centroid, par_id++, 1);
    }
    subdepth[centroid].assign(par_id, 1);
    subbit[centroid].resize(par_id);

    while(!que.empty()) {
      int idx, p, top, dep;
      tie(idx, p, top, dep) = que.front();
      que.pop();
      chmax(depth[centroid], dep);
      chmax(subdepth[centroid][top], dep);
      belong[idx].emplace_back(centroid, dep, top);
      for(auto &to : g[idx]) {
        if(used[to] || to == p) continue;
        que.emplace(to, idx, top, dep + 1);
      }
    }
    bit[centroid] = BinaryIndexedTree< int64 >(depth[centroid] + 2);
    for(int i = 0; i < par_id; i++) {
      subbit[centroid][i] = BinaryIndexedTree< int64 >(subdepth[centroid][i] + 2);
    }
    for(auto &to : t[centroid]) rec(to);
    used[centroid] = false;
  })(cd.build(t));

  while(Q--) {
    int X, Y, Z;
    cin >> X >> Y >> Z;
    --X;
    int64 ret = 0;
    for(auto &p : belong[X]) {
      int centroid, dep, top;
      tie(centroid, dep, top) = p;
      ret += bit[centroid].sum(dep);
      if(top >= 0) ret += subbit[centroid][top].sum(dep);

      int rest_dep = Y - dep;
      if(rest_dep < 0) continue;

      bit[centroid].add(0, Z);
      bit[centroid].add(min(depth[centroid], rest_dep) + 1, -Z);
      if(top >= 0) {
        subbit[centroid][top].add(1, -Z);
        subbit[centroid][top].add(min(subdepth[centroid][top], rest_dep) + 1, +Z);
      }
    }
    cout << ret << "\n";
  }
}

