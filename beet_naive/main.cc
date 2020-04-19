#include <bits/stdc++.h>
using namespace std;
template<typename T1,typename T2> inline void chmin(T1 &a,T2 b){if(a>b) a=b;}
template<typename T1,typename T2> inline void chmax(T1 &a,T2 b){if(a<b) a=b;}
using Int = long long;
const char newl = '\n';


template<typename F>
struct FixPoint : F{
  FixPoint(F&& f):F(forward<F>(f)){}
  template<typename... Args>
  decltype(auto) operator()(Args&&... args) const{
    return F::operator()(*this,forward<Args>(args)...);
  }
};
template<typename F>
inline decltype(auto) MFP(F&& f){
  return FixPoint<F>{forward<F>(f)};
}

//INSERT ABOVE HERE
signed main(){
  cin.tie(0);
  ios::sync_with_stdio(0);

  int n,q;
  cin>>n>>q;
  vector<vector<int>> G(n);
  for(int i=1;i<n;i++){
    int a,b;
    cin>>a>>b;
    a--;b--;
    G[a].emplace_back(b);
    G[b].emplace_back(a);
  }

  using ll = long long;
  vector<ll> vs(n,0);
  for(int i=0;i<q;i++){
    int x,y,z;
    cin>>x>>y>>z;
    x--;
    cout<<vs[x]<<newl;
    MFP([&](auto dfs,int v,int p,int d)->void{
      vs[v]+=z;
      if(d==y) return;
      for(int u:G[v])
        if(u!=p) dfs(u,v,d+1);
    })(x,-1,0);
  }

  return 0;
}
