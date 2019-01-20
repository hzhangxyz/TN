#include <iostream>
#include "tensor.cpp"

using T=Tensor<double,DefaultLeg>;

int main(){
  auto t1 = new T {2,{2,3},{DefaultLeg::Left,DefaultLeg::Right}};
  t1->fill({1.2,3.4,6.4, 3.56,8.54,67.7});

  std::cout << *t1;
  auto p2 = new T {2.3};
  std::cout << *p2;
  return 0;
}
