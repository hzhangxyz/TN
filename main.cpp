#include <iostream>
#include "tensor.cpp"

using T=Tensor<double,DefaultLeg, int>;

int label(){
  static int i = 0;
  return i++;
}
#define print std::cout << "#[" << label() << "] "

T return_a_tensor(float i){
  T res(i);
  return res;
}

using Leg = DefaultLeg;

int main(){
  {/* Tensor(); */
    T t;
    print << t << "\n";
  }
  {/* Tensor(Tensor const& t);
      Tensor(Tensor& t);
      !Tensor(Tensor&& t);
      Tensor(Base d); */
    T a (1.2);
    print << a << "\n";
    T b (a);
    print << a.data << "\n";
    print << b.data << "\n";
    T c (const_ref(b));
    print << b.data << "\n";
    print << c.data << "\n";
    T d (return_a_tensor(2.3));
    print << d << "\n";
  }
  {/* Tensor(std::vector<Base> const& d, Leg n);
      Tensor(int s, const Base* const& d, Leg n);
      Tensor(int s, Base*& d, Leg n);
      !Tensor(int s, Base*&& d, Leg n)a; */
    T a ({1,2,3,4.5}, Leg::Left);
    print << a << "\n";
    double* b = new double[3] {10.,2.,3.};
    print << b << "\n";
    T c (3, b, Leg::Right);
    print << b << "\n";
    print << c << "\n";
    print << c.data << "\n";
    b = new double[3] {0.5,3.7,9.0};
    print << b << "\n";
    T d (3, const_ref(b), Leg::Right);
    print << b << "\n";
    print << d << "\n";
    print << d.data << "\n";
    T* e = new T[2] {{4.3}, {{3.5,7.9}, Leg::Phy}};
    print << e[0] << "\n";
    print << e[1] << "\n";
    delete[] e;
    double f[] = {0.5,3.7,9.0};
    T g (3, const_ref(f), Leg::Right);
  }
  {/* Tensor(Idim r, std::vector<Idim> const& d, std::vector<Leg> const& n, std::vector<Base> const& dd);
      Tensor(Idim r, std::vector<Idim> const& d, std::vector<Leg> const& n, const Base* const& dd);
      Tensor(Idim r, std::vector<Idim> const& d, std::vector<Leg> const& n, Base*& dd);
      Tensor(Idim r, std::vector<Idim> const& d, std::vector<Leg> const& n, Base*&& dd);*/
    double* a = new double[6] {2,3,4,5,6,7};
    double *const b = new double[6] {3,4,5,6,7,8};
    auto t = new T[3] {
                    {2, {2,3}, {Leg::Left1,Leg::Right2},{1,2,3,4,5,6}},
                    {2, {2,3}, {Leg::Left1,Leg::Right2},a},
                    {2, {2,3}, {Leg::Left1,Leg::Right2},b}
                  };
    print << t[0] << "\n";
    print << t << "\n";
    delete[] t;
    print << t << "\n";
    print << a << "\n";
    print << b << "\n";
  }
  return 0;
}
