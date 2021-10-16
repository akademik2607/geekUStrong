#include<iostream>

void showPointerInfo(int* p, std::string pName){
  std::cout << pName << " adress: " << p << "; value: " << *p << std::endl;
}

//1.
template<class T>
void swap(T& p1, T& p2){
  T temp;
  temp = p1;
  p1 = p2;
  p2 = temp;
}

//2


int main(){
  //1.
  int* a = new int;
  int* b = new int;

  *a = 2;
  *b = 5;

  showPointerInfo(a, "a");
  showPointerInfo(b, "b");
  swap(a, b);
  std::cout << "====================" << std::endl;
  showPointerInfo(a, "a");
  showPointerInfo(b, "b");


  return 0;
}