/**
 * @file LPHelperObject.hpp
 * LPObject is the base Class for LPProject
 * @brief Header file for LPHelperObject class
 * @details This file contains the declaration of the LPHelperObject class.
 *
 */
#pragma once
#include <string>

class LPHelperObject
{
private:
public:
  std::string m_accessToken;

  LPHelperObject();
  ~LPHelperObject();
};
