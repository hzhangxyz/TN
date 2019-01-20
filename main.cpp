#include <iostream>
#include "tensor.cpp"

using T=Tensor<double,DefaultLeg>;

int main(){
  auto t1 = new T {2,{2,3},{DefaultLeg::Left,DefaultLeg::Right},{1.2,3.4,6.4, 3.56,8.54,67.7}};
  std::cout << *t1;
  auto p2 = new T {2.3};
  std::cout << *p2;
  int d[] = {1,1};
  auto p3 = new T {2,std::vector<int>{d,d+2},{DefaultLeg::Left,DefaultLeg::Right},{1.2}};
  std::cout << *p3;
  std::vector<int> tt {d,d+2};
  *tt.end() = 2;
  std::cout << d[1] << "\n";
  auto p4 = new T {{1,2,3},DefaultLeg::Phy};
  std::cout << *p4;
  return 0;
}
