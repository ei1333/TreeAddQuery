#include "testlib.h"
#include <bits/stdc++.h>

using namespace std;

const int MIN_N = 1;
const int MAX_N = 1e5;
const int MIN_Q = 1;
const int MAX_Q = 1e5;
const int MIN_Z = 1;
const int MAX_Z = 1e9;

struct UnionFind {
  int num;
  vector<int> rs, ps;
  UnionFind() {}
  UnionFind(int n) : num(n), rs(n, 1), ps(n, 0) {
    iota(ps.begin(), ps.end(), 0);
  }
  int find(int x) { return (x == ps[x] ? x : ps[x] = find(ps[x])); }
  bool same(int x, int y) { return find(x) == find(y); }
  void unite(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y)
      return;
    if (rs[x] < rs[y])
      swap(x, y);
    rs[x] += rs[y];
    ps[y] = x;
    num--;
  }
  int size(int x) { return rs[find(x)]; }
  int count() const { return num; }
};

int main() {
  registerValidation();
  int N = inf.readInt(MIN_N, MAX_N, "N");
  inf.readSpace();
  int Q = inf.readInt(MIN_Q, MAX_Q, "Q");
  inf.readEoln();

  UnionFind uf(N);
  for (int i = 1; i < N; i++) {
    int A = inf.readInt(1, N, "A");
    inf.readSpace();
    int B = inf.readInt(1, N, "B");
    inf.readEoln();
    A--;
    B--;
    uf.unite(A, B);
  }
  ensure(uf.count() == 1);

  for (int i = 0; i < Q; i++) {
    inf.readInt(1, N, "X");
    inf.readSpace();
    inf.readInt(0, N - 1, "Y");
    inf.readSpace();
    inf.readInt(MIN_Z, MAX_Z, "Z");
    inf.readEoln();
  }

  inf.readEof();
  return 0;
}
