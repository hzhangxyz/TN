#include <random>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <map>

enum class DefaultLeg {Left, Right, Down, Up, Phy, Left1, Right1, Down1, Up1, Phy1, Left2, Right2, Down2, Up2, Phy2, Left3, Right3, Down3, Up3, Phy3, Left4, Right4, Down4, Up4, Phy4};

template <class Base, class Leg>
class Tensor{
public:
  int rank=0;
  std::vector<int> dim;
  std::vector<Leg> name;
  Base* data=nullptr;

  int size=1;

  Tensor();
  Tensor(Tensor&& t);//move
  Tensor(const Base d);//d1
  Tensor(const std::vector<Base>& d, Leg n);
  //Tensor(std::vector<Base>& d, Leg n); //vector转换到指针有点悬
  //Tensor(const Base* d, Leg n); //cpp并不支持这种形式重载
  Tensor(Base*& d, Leg n);//d2
  Tensor(int r, const std::vector<int>& d, const std::vector<Leg>& n, const std::vector<Base>& dd);
  //Tensor(int r, const std::vector<int>& d, const std::vector<Leg>& n, std::vector<Base>& dd);
  //Tensor(int r, const std::vector<int>& d, const std::vector<Leg>& n, const Base* dd);
  Tensor(int r, const std::vector<int>& d, const std::vector<Leg>& n, Base*& dd);//dn

  ~Tensor();

  static Tensor contract(Tensor tensor1,
                         Tensor tensor2,
                         std::vector<Leg> leg1,
                         std::vector<Leg> leg2,
                         std::map<Leg,Leg> map1,
                         std::map<Leg,Leg> map2);
};

// Constructor
template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(){
}

template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(Tensor&& t){
  rank = t.rank;
  dim.swap(t.dim);
  name.swap(t.name);
  data = t.data;
  t.data = nullptr;
}

template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(const Base d){
  data = new Base[1];
  data[1] = d;
}

#define COMMON_INIT {\
rank = 1;\
size = d.size;\
dim.push_back(size);\
name.push_back(n);}

template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(const std::vector<Base>& d, Leg n){
  COMMON_INIT;
  data = (Base*)malloc(sizeof(Base)*size);
  std::copy(d.begin(),d.begin()+size,data);
}

template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(Base*& d, Leg n){
  COMMON_INIT;
  data = d;
  d = nullptr;
}
#undef COMMON_INIT

#define COMMON_INIT {\
rank = r;\
dim = d;\
name = n;\
for(int i=0;i<rank;i++)size*=dim[i];}

template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(int r,
                          const std::vector<int>& d,
                          const std::vector<Leg>& n,
                          const std::vector<Base>& dd){
  COMMON_INIT;
  data = (Base*)malloc(sizeof(Base)*size);
  std::copy(dd.begin(),dd.begin()+size,data);
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
Tensor<Base,Leg> Tensor<Base,Leg>::contract(Tensor tensor1,
                                            Tensor tensor2,
                                            std::vector<Leg> leg1,
                                            std::vector<Leg> leg2,
                                            std::map<Leg,Leg> map1,
                                            std::map<Leg,Leg> map2){
}
