#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>

// *只用于data
// 能变则变,调用T&&, T& 等同于move此变量
// 调用const T&才会复制

enum class DefaultLeg {Left, Right, Down, Up, Phy, Left1, Right1, Down1, Up1, Phy1, Left2, Right2, Down2, Up2, Phy2, Left3, Right3, Down3, Up3, Phy3, Left4, Right4, Down4, Up4, Phy4};

#define const_ref(x) ((decltype(x) const&)(x))
// 表达式不需要const_ref, 所以这里x一定是单个变量

template <class Base, class Leg, class Idim>
class Tensor{
public:
  Idim rank=0;
  std::vector<Idim> dim;
  std::vector<Leg> name;
  Base* data=nullptr;

  Idim size=1;

  Tensor();
  Tensor(Tensor const& t);//copy
  Tensor(Tensor& t);//move
  Tensor(Tensor&& t);
  Tensor(Base d);//d1
  Tensor(std::vector<Base> const& d, Leg n);// 完全和const_ref(d.begin())一样,只不过这样方便用{}初始化
  Tensor(Idim s, const Base* const& d, Leg n);
  Tensor(Idim s, Base*& d, Leg n);
  Tensor(Idim s, Base*&& d, Leg n);//d2
  Tensor(Idim r, std::vector<Idim> const& d, std::vector<Leg> const& n, std::vector<Base> const& dd);
  Tensor(Idim r, std::vector<Idim> const& d, std::vector<Leg> const& n, const Base* const& dd);
  Tensor(Idim r, std::vector<Idim> const& d, std::vector<Leg> const& n, Base*& dd);
  Tensor(Idim r, std::vector<Idim> const& d, std::vector<Leg> const& n, Base*&& dd);//dn

  ~Tensor();

  static Tensor contract(Tensor const& tensor1,
                         Tensor const& tensor2,
                         std::vector<Leg> const& leg1,
                         std::vector<Leg> const& leg2,
                         std::map<Leg,Leg> const& map1,
                         std::map<Leg,Leg> const& map2);

  static Tensor Qr_s_Q(Tensor const& tensor);
};

// Constructor
template <class Base, class Leg, class Idim>
Tensor<Base, Leg, Idim>::Tensor(){
  new (this) Tensor {0};
}

template <class Base, class Leg, class Idim>
Tensor<Base, Leg, Idim>::Tensor(Tensor const& t){
  rank = t.rank;
  dim = t.dim;
  name = t.name;
  size = t.size;
  data = (Base*)malloc(sizeof(Base)*size);
  std::copy(t.data,t.data+size,data);
}

template <class Base, class Leg, class Idim>
Tensor<Base, Leg, Idim>::Tensor(Tensor& t){
  rank = t.rank;
  size = t.size;
  dim.swap(t.dim);
  name.swap(t.name);
  data = t.data;
  t.data = nullptr;
}

template <class Base, class Leg, class Idim>
Tensor<Base, Leg, Idim>::Tensor(Tensor&& t){
  new (this) Tensor {t};
}

template <class Base, class Leg, class Idim>
Tensor<Base, Leg, Idim>::Tensor(Base d){
  data = new Base[1];
  data[0] = d;
}

template <class Base, class Leg, class Idim>
Tensor<Base, Leg, Idim>::Tensor(std::vector<Base> const& d, Leg n){
  Idim s = d.size();
  new (this) Tensor {s, const_ref(d.data()),n};
}

#define COMMON_INIT {\
size = s;\
rank = 1;\
dim.push_back(size);\
name.push_back(n);}

template <class Base, class Leg, class Idim>
Tensor<Base, Leg, Idim>::Tensor(Idim s, const Base* const& d, Leg n){
  COMMON_INIT;
  //data = (Base*)malloc(sizeof(Base)*size);
  data = new Base[size];
  std::copy(d,d+size,data);
}

template <class Base, class Leg, class Idim>
Tensor<Base, Leg, Idim>::Tensor(Idim s, Base*& d, Leg n){
  COMMON_INIT;
  data = d;
  d = nullptr;
}
#undef COMMON_INIT

template <class Base, class Leg, class Idim>
Tensor<Base, Leg, Idim>::Tensor(Idim s, Base*&& d, Leg n){
  new (this) Tensor {s, d, n};
}

template <class Base, class Leg, class Idim>
Tensor<Base, Leg, Idim>::Tensor(Idim r,
                          std::vector<Idim> const& d,
                          std::vector<Leg> const& n,
                          std::vector<Base> const& dd){
  new (this) Tensor {r, d, n, const_ref(dd.data())};
}

#define COMMON_INIT {\
rank = r;\
dim = d;\
name = n;\
for(Idim i=0;i<rank;i++)size*=dim[i];}

template <class Base, class Leg, class Idim>
Tensor<Base, Leg, Idim>::Tensor(Idim r,
                          std::vector<Idim> const& d,
                          std::vector<Leg> const& n,
                          const Base* const& dd){
  COMMON_INIT;
  //data = (Base*)malloc(sizeof(Base)*size);
  data = new Base[size];
  std::copy(dd,dd+size,data);
}

template <class Base, class Leg, class Idim>
Tensor<Base, Leg, Idim>::Tensor(Idim r,
                          std::vector<Idim> const& d,
                          std::vector<Leg> const& n,
                          Base*& dd){
  COMMON_INIT;
  data = dd;
  dd = nullptr;
}
#undef COMMON_INIT

template <class Base, class Leg, class Idim>
Tensor<Base, Leg, Idim>::Tensor(Idim r,
                          std::vector<Idim> const& d,
                          std::vector<Leg> const& n,
                          Base*&& dd){
  new (this) Tensor {r, d, n, dd};
}

// Destructor
template <class Base, class Leg, class Idim>
Tensor<Base, Leg, Idim>::~Tensor(){
  //if(data!=nullptr) free(data);
  if(data!=nullptr) delete[] data;
}

// Print
template <class Base, class Leg, class Idim>
std::ostream& operator<<(std::ostream& os, const Tensor<Base, Leg, Idim>& obj){
  os << "rank=" << obj.rank << " dim=[";
  for(Idim i=0;i<obj.rank;i++){
    os << "" << obj.dim[i] << ", ";
  }
  os << "] data=[";
  for(Idim i=0;i<obj.size;i++){
    os << obj.data[i] << ", ";
  }
  os << "]";
  return os;
}

// Contract
#define get_index(it,vec,tmp, res) {\
auto tmp = std::find(vec.begin(), vec.end(), it);\
if(tmp==vec.end()){\
  res = -1;\
}else{\
  res = std::distance(vec.begin(),tmp);\
}}

template <class Base, class Leg, class Idim>
Tensor<Base, Leg, Idim> Tensor<Base, Leg, Idim>::contract(Tensor const& tensor1,
                                                          Tensor const& tensor2,
                                                          std::vector<Leg> const& leg1,
                                                          std::vector<Leg> const& leg2,
                                                          std::map<Leg,Leg> const& map1,
                                                          std::map<Leg,Leg> const& map2){
  std::vector<std::pair<Idim,Idim>> order;
  std::vector<Leg> true_leg1, true_leg2;
  auto contract_length = leg1.size();
  Idim res_rank = tensor1.rank + tensor2.rank;
  for(auto i=0;i<contract_length;i++){
    Idim index1, index2;
    get_index(leg1[i], tensor1.name, l1, index1);
    if(index1==-1)continue;
    get_index(leg2[i], tensor2.name, l2, index2);
    if(index2==-1)continue;
    order.push_back({index1, index2});
    true_leg1.push_back(leg1[i]);
    true_leg2.push_back(leg2[i]);
    res_rank -= 2;
  }
  std::vector<Idim> res_dim;
  std::vector<Leg> res_name;
  for(auto i=0;i<tensor1.rank;i++){

  }
}

#undef get_index

// Qr
