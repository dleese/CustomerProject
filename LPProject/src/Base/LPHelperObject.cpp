/**
 * @file LPHelperObject.cpp
 * @brief Implementation of LPHelperObject class
 * @details This file contains the implementation of the LPHelperObject class methods.
 * @author Dirk Leese
 * @date 2025
 */

#include "LPHelperObject.hpp"
#include <iostream>

namespace logipad
{
  namespace core
  {

    /**
     * @brief Default constructor implementation
     * @details Initializes a new HelperObject instance. The access token member
     *          is automatically initialized as an empty string by the std::string default constructor.
     *          Prints a debug message to indicate constructor invocation.
     */
    HelperObject::HelperObject()
    {
      std::cout << "Constructor called" << std::endl;
    }

    /**
     * @brief Destructor implementation
     * @details Cleans up the HelperObject instance. All members are automatically
     *          destroyed. Currently only prints a debug message to indicate destructor invocation.
     */
    HelperObject::~HelperObject()
    {
      std::cout << "Destructor called" << std::endl;
    }

  } // namespace core
} // namespace logipad
