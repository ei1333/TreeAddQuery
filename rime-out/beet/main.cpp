#include<bits/stdc++.h>
using namespace std;
using Int = long long;
template<typename T1,typename T2> inline void chmin(T1 &a,T2 b){if(a>b) a=b;}
template<typename T1,typename T2> inline void chmax(T1 &a,T2 b){if(a<b) a=b;}


struct FastIO{
  FastIO(){
    cin.tie(0);
    ios::sync_with_stdio(0);
  }
}fastio_beet;


struct Centroid{
  vector<int> sz,dead;
  vector< vector<int> > G;
  Centroid(){}
  Centroid(int n):sz(n,1),dead(n,0),G(n){}

  void add_edge(int u,int v){
    G[u].emplace_back(v);
    G[v].emplace_back(u);
  }

  int dfs(int v,int p){
    sz[v]=1;
    for(int u:G[v])
      if(u!=p&&!dead[u]) sz[v]+=dfs(u,v);
    return sz[v];
  }

  void find(int v,int p,int tmp,vector<int> &cs) {
    int ok=1;
    for (int u:G[v]){
      if(u==p||dead[u]) continue;
      find(u,v,tmp,cs);
      ok&=(sz[u]<=tmp/2);
    }
    ok&=(tmp-sz[v]<=tmp/2);
    if(ok) cs.push_back(v);
  }

  vector<int> build(int r) {
    int tmp=dfs(r,-1);
    vector<int> cs;
    find(r,-1,tmp,cs);
    return cs;
  }

  void disable(int v){
    dead[v]=1;
  }

  void enable(int v){
    dead[v]=0;
  }

  int alive(int v){
    return !dead[v];
  }
};


template <typename E, typename H>
struct SegmentTree{
  // using H = function<E(E,E)>;
  int n,height;
  H h;
  E ei;
  vector<E> laz;

  SegmentTree(H h,E ei):h(h),ei(ei){}

  void init(int n_){
    n=1;height=0;
    while(n<n_) n<<=1,height++;
    laz.assign(2*n,ei);
  }

  void reset(int n_){
    n=1;height=0;
    while(n<n_) n<<=1,height++;
    fill(laz.begin(),laz.begin()+2*n,ei);
  }

  inline void propagate(int k){
    if(laz[k]==ei) return;
    laz[(k<<1)|0]=h(laz[(k<<1)|0],laz[k]);
    laz[(k<<1)|1]=h(laz[(k<<1)|1],laz[k]);
    laz[k]=ei;
  }

  inline void thrust(int k){
    for(int i=height;i;i--) propagate(k>>i);
  }

  void update(int a,int b,E x){
    if(a>=b) return;
    thrust(a+=n);
    thrust(b+=n-1);
    for(int l=a,r=b+1;l<r;l>>=1,r>>=1){
      if(l&1) laz[l]=h(laz[l],x),l++;
      if(r&1) --r,laz[r]=h(laz[r],x);
    }
  }

  E get_val(int a){
    thrust(a+=n);
    return laz[a];
  }

  void set_val(int a,E x){
    thrust(a+=n);
    laz[a]=x;
  }
};


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
  int n,q;
  cin>>n>>q;

  Centroid G(n);
  for(int i=1;i<n;i++){
    int a,b;
    cin>>a>>b;
    a--;b--;
    G.add_edge(a,b);
  }

  vector<int> xs(q),ys(q),zs(q);
  for(int i=0;i<q;i++) cin>>xs[i]>>ys[i]>>zs[i];

  vector<vector<int>> H(n);
  for(int i=0;i<q;i++){
    xs[i]--;
    H[xs[i]].emplace_back(i);
  }

  vector<int> dep(n);

  using ll = long long;
  vector<ll> ans(q,0);
  auto h=[&](ll a,ll b){return a+b;};
  SegmentTree<ll, decltype(h)> seg(h,0);
  seg.init(n*2);

  queue<int> que;
  que.emplace(G.build(0)[0]);
  while(!que.empty()){
    int r=que.front();que.pop();
    dep[r]=0;

    // add for all
    {
      int len=1;
      vector<int> qs(H[r]);
      for(int t:G.G[r]){
        if(!G.alive(t)) continue;
        MFP([&](auto dfs,int v,int p,int d)->void{
          dep[v]=d;
          chmax(len,d+1);
          for(int i:H[v]) qs.emplace_back(i);
          for(int u:G.G[v]){
            if(u==p) continue;
            if(!G.alive(u)) continue;
            dfs(u,v,d+1);
          }
        })(t,r,1);
      }
      seg.reset(len);
      sort(qs.begin(),qs.end());
      for(int i:qs){
        ans[i]+=seg.get_val(dep[xs[i]]);
        if(ys[i]>=dep[xs[i]])
          seg.update(0,min(len,ys[i]-dep[xs[i]]+1),zs[i]);
      }
    }

    // sub for subtree
    {
      for(int t:G.G[r]){
        if(!G.alive(t)) continue;
        int len=1;
        vector<int> qs;
        MFP([&](auto dfs,int v,int p,int d)->void{
          dep[v]=d;
          chmax(len,d+1);
          for(int i:H[v]) qs.emplace_back(i);
          for(int u:G.G[v]){
            if(u==p) continue;
            if(!G.alive(u)) continue;
            dfs(u,v,d+1);
          }
        })(t,r,1);
        seg.reset(len);
        sort(qs.begin(),qs.end());
        for(int i:qs){
          ans[i]-=seg.get_val(dep[xs[i]]);
          if(ys[i]>=dep[xs[i]])
            seg.update(0,min(len,ys[i]-dep[xs[i]]+1),zs[i]);
        }
      }
    }

    G.disable(r);
    for(int t:G.G[r])
      if(G.alive(t))
        que.emplace(G.build(t)[0]);
  }

  for(auto a:ans) cout<<a<<"\n";
  cout<<flush;
  return 0;
}
