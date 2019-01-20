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
  int* dim=nullptr;
  Leg* name=nullptr;
  Base* data=nullptr;

  int size=1;

  Tensor(int _rank, std::vector<int> _dim, std::vector<Leg> _name);
  Tensor(int _rank, int* _dim, std::vector<Leg> _name);
  Tensor(int _rank, std::vector<int> _dim, Leg* _name);
  Tensor(int _rank, int* _dim, Leg* _name);
  Tensor* fill(const std::vector<Base> d);
  Tensor* fill(const Base* d);

  Tensor(const Base d);

  Tensor(Tensor&& t);//move
  ~Tensor();
  Tensor();

  static Tensor contract(Tensor tensor1,
                         Tensor tensor2,
                         std::vector<Leg> leg1,
                         std::vector<Leg> leg2,
                         std::map<Leg,Leg> map1,
                         std::map<Leg,Leg> map2);
};

template <class Base, class Leg>
Tensor<Base,Leg> Tensor<Base,Leg>::contract(Tensor tensor1,
                                                   Tensor tensor2,
                                                   std::vector<Leg> leg1,
                                                   std::vector<Leg> leg2,
                                                   std::map<Leg,Leg> map1,
                                                   std::map<Leg,Leg> map2){
}

template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(){
}

template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(const Base d){
  data = new Base[1];
  *data = d;
}

template <class Base, class Leg>
Tensor<Base, Leg>* Tensor<Base, Leg>::fill(const Base* d){
  std::copy(d,d+size,data);
  return this;
}

template <class Base, class Leg>
Tensor<Base, Leg>* Tensor<Base, Leg>::fill(const std::vector<Base> d){
  std::copy(d.begin(),d.begin()+size,data);
  return this;
}

template <class Base, class Leg>
std::ostream& operator<<(std::ostream& os, const Tensor<Base, Leg>& obj){
  os << obj.rank << " [";
  for(int i=0;i<obj.rank;i++){
    os << "" << obj.dim[i] << ", ";
  }
  os << "]\n";
  for(int i=0;i<obj.size;i++){
    os << obj.data[i] << ",";
  }
  os << "\n";
  return os;
}

template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(int _rank,
       std::vector<int> _dim,
       std::vector<Leg> _name){
  rank = _rank;
  dim = new int[rank];
  name = new Leg[rank];
  std::copy(_dim.begin(),_dim.end(),dim);
  std::copy(_name.begin(),_name.end(),name);
  for(int i=0;i<rank;i++)size*=dim[i];
  data = (Base*)malloc(sizeof(Base)*size);
}

template <class Base, class Leg>
Tensor<Base, Leg>::Tensor(Tensor&& t){
  rank = t.rank;
  dim = t.dim;
  name = t.name;
  data = t.data;
  t.dim = nullptr;
  t.name = nullptr;
  t.data = nullptr;
}

template <class Base, class Leg>
Tensor<Base, Leg>::~Tensor(){
  if(dim!=nullptr) delete[] dim;
  if(name!=nullptr) delete[] name;
  if(data!=nullptr) free(data);
}
