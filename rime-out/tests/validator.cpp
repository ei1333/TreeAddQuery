#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

// 制約
const int N_MIN = 1;
const int N_MAX = 100000;
const int Q_MIN = 1;
const int Q_MAX = 100000;
const int A_MIN = 1;
const int A_MAX = 1000000000;


struct UnionFind {
  vector< int > data;
 
  UnionFind(int sz) {
    data.assign(sz, -1);
  }
 
  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if(x == y) return (false);
    if(data[x] > data[y]) swap(x, y);
    data[x] += data[y];
    data[y] = x;
    return (true);
  }
 
  int find(int k) {
    if(data[k] < 0) return (k);
    return (data[k] = find(data[k]));
  }
 
  int size(int k) {
    return (-data[find(k)]);
  }
};


void input() {
  int N = inf.readInt(N_MIN, N_MAX, "N");
  inf.readSpace();
  int Q = inf.readInt(Q_MIN, Q_MAX, "Q");
  inf.readEoln();
  UnionFind uf(N);
  for(int i = 1; i < N; i++) {
    int A = inf.readInt(1, N, "A");
    inf.readSpace();
    int B = inf.readInt(1, N, "B");
    inf.readEoln();
    assert(uf.unite(A - 1, B - 1));
  }
  for(int i = 0; i < Q; i++){ 
    int X = inf.readInt(1, N, "X");
    inf.readSpace();
    int Y = inf.readInt(0, N - 1, "Y");
    inf.readSpace();
    int Z = inf.readInt(A_MIN, A_MAX, "Z");
    inf.readEoln();
  } 
  inf.readEof();
}

int main() {
  registerValidation();
  input();
}
