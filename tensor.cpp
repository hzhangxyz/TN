#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>

// *只用于data
// 能变则变,调用T&&, T& 等同于move此变量
// 调用const T&才会复制

enum class DefaultLeg {Left, Right, Down, Up, Phy, Left1, Right1, Down1, Up1, Phy1, Left2, Right2, Down2, Up2, Phy2, Left3, Right3, Down3, Up3, Phy3, Left4, Right4, Down4, Up4, Phy4};

#define const_ref(x) ((const decltype(x) &)(x))
// 表达式不需要const_ref, 所以这里x一定是单个变量

template <class Base, class Leg>
class Tensor{
public:
  int rank=0;
  std::vector<int> dim;
  std::vector<Leg> name;
  Base* data=nullptr;

  int size=1;

  Tensor();
  Tensor(const Tensor& t);//copy
  Tensor(Tensor& t);//move
  Tensor(Tensor&& t);
  Tensor(Base d);//d1
  Tensor(const std::vector<Base>& d, Leg n);// 完全和const_ref(d.begin())一样,只不过这样方便用{}初始化
  Tensor(int s, const Base*& d, Leg n);
  Tensor(int s, Base*& d, Leg n);
  Tensor(int s, Base*&& d, Leg n);//d2
  Tensor(int r, const std::vector<int>& d, const std::vector<Leg>& n, const std::vector<Base>& dd);
  Tensor(int r, const std::vector<int>& d, const std::vector<Leg>& n, const Base*& dd);
  Tensor(int r, const std::vector<int>& d, const std::vector<Leg>& n, Base*& dd);
  Tensor(int r, const std::vector<int>& d, const std::vector<Leg>& n, Base*&& dd);//dn

  ~Tensor();

  static Tensor contract(const Tensor& tensor1,
                          const Tensor& tensor2,
                          const std::vector<Leg>& leg1,
                          const std::vector<Leg>& leg2,
                          const std::map<Leg,Leg>& map1,
                          const std::map<Leg,Leg>& map2);
  
  static Tensor Qr_s_Q(const Tensor& tensor);
};

// Constructor
template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(){
}

template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(const Tensor<Base, Leg>& t){
  rank = t.rank;
  dim = t.dim;
  name = t.name;
  size = t.size;
  data = (Base*)malloc(sizeof(Base)*size);
  std::copy(t.data,t.data+size,data);
}

template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(Tensor<Base, Leg>& t){
  rank = t.rank;
  size = t.size;
  dim.swap(t.dim);
  name.swap(t.name);
  data = t.data;
  t.data = nullptr;
}

template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(Tensor<Base, Leg>&& t){
  new (this) Tensor(t);
}

template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(Base d){
  data = new Base[1];
  data[1] = d;
}

template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(const std::vector<Base>& d, Leg n){
  /*size = d.size();
  COMMON_INIT;
  data = (Base*)malloc(sizeof(Base)*size);
  std::copy(&d.front(),&d.front()+size,data);*/
  new (this) Tensor(d.size(), (Base*)&d.front(),n);
}

#define COMMON_INIT {\
size = s;\
rank = 1;\
dim.push_back(size);\
name.push_back(n);}

template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(int s, const Base*& d, Leg n){
  COMMON_INIT;
  data = (Base*)malloc(sizeof(Base)*size);
  std::copy(d,d+size,data);
}

template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(int s, Base*& d, Leg n){
  COMMON_INIT;
  data = d;
  d = nullptr;
}
#undef COMMON_INIT

template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(int s, Base*&& d, Leg n){
  new (this) Tensor(s, d, n);
}

template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(int r,
                          const std::vector<int>& d,
                          const std::vector<Leg>& n,
                          const std::vector<Base>& dd){
  /*COMMON_INIT;
  data = (Base*)malloc(sizeof(Base)*size);
  std::copy(&dd.front(),&dd.front()+size,data);*/
  new (this) Tensor(r, d, n, (Base*)&dd.front());
}

#define COMMON_INIT {\
rank = r;\
dim = d;\
name = n;\
for(int i=0;i<rank;i++)size*=dim[i];}

template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(int r,
                          const std::vector<int>& d,
                          const std::vector<Leg>& n,
                          const Base*& dd){
  COMMON_INIT;
  data = (Base*)malloc(sizeof(Base)*size);
  std::copy(dd,dd+size,data);
}

template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(int r,
                          const std::vector<int>& d,
                          const std::vector<Leg>& n,
                          Base*& dd){
  COMMON_INIT;
  data = dd;
  dd = nullptr;
}
#undef COMMON_INIT

template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(int r,
                          const std::vector<int>& d,
                          const std::vector<Leg>& n,
                          Base*&& dd){
  new (this) Tensor(r, d, n, dd);
}

// Destructor
template <class Base, class Leg>
Tensor<Base, Leg>::~Tensor(){
  if(data!=nullptr) free(data);
}

// Print
template <class Base, class Leg>
std::ostream& operator<<(std::ostream& os, const Tensor<Base, Leg>& obj){
  os << obj.rank << " [";
  for(int i=0;i<obj.rank;i++){
    os << "" << obj.dim[i] << ", ";
  }
  os << "]\n";
  for(int i=0;i<obj.size;i++){
    os << obj.data[i] << ", ";
  }
  os << "\n";
  return os;
}

// Contract
template <class Base, class Leg>
Tensor<Base,Leg> Tensor<Base,Leg>::contract(const Tensor& tensor1,
                                             const Tensor& tensor2,
                                             const std::vector<Leg>& leg1,
                                             const std::vector<Leg>& leg2,
                                             const std::map<Leg,Leg>& map1,
                                             const std::map<Leg,Leg>& map2){
}

// Qr