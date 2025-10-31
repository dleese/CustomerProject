/// LPHelperObject.cpp
#include "LPHelperObject.hpp"
#include <iostream>
/**
  LPObject constructor
  @param a the first
  @pram b the second
  @result the reslut
*/
LPHelperObject::LPHelperObject() {
  std::cout << "Constructor called" << std::endl;  // constructor implementation
}

/**
  LPHelperObject destructor
  @param a the first
  @pram b the second
  @result the reslut
*/
LPHelperObject::~LPHelperObject(){
  std::cout << "Destructor called" << std::endl;
}
