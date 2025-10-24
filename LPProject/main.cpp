#include "Version.hpp"
#include <iostream>
#include "LPObject.hpp"

int main() {
  std::cout << "Quick check if things work.\n"; 
  std::cout << "Project Version:" << LP_PROJECT_VERSION_MAJOR //  
                                  << "." << LP_PROJECT_VERSION_MINOR //
                                  << "." << LP_PROJECT_VERSION_PATCH //
                                  << std::endl;

  
  LPObject *ptr_lpObj = new LPObject();
  delete ptr_lpObj;
  
  ptr_lpObj = nullptr;

  return 0;
}
