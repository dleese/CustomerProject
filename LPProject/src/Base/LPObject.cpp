/// LPObject.cpp
#include "LPObject.hpp"
#include <iostream>

LPObject::LPObject() {
  std::cout << "Constructor called" << std::endl;  // constructor implementation
}

LPObject::~LPObject(){
  std::cout << "Destructor called" << std::endl;
}
