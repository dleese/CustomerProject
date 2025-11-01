/**
 * @file LPHelperObject.hpp
 * @brief Header file for LPHelperObject class
 * @details This file contains the declaration of the LPHelperObject class.
 *          LPHelperObject is the base helper class for LPProject that provides
 *          basic functionality for storing access tokens and managing authentication state.
 * @author Dirk Leese
 * @date 2025
 */

#pragma once

#include <string>

/**
 * @namespace logipad
 * @brief Root namespace for all Logipad project code
 */

/**
 * @namespace logipad::core
 * @brief Core functionality and base classes
 */

namespace logipad
{
  namespace core
  {

    /**
     * @class HelperObject
     * @brief Base helper class for LPProject
     * @details This class provides basic functionality for managing access tokens
     *          and serves as a foundation for other classes in the LPProject system.
     */
    class HelperObject
    {
    private:
    public:
      /**
       * @brief Access token for authentication
       * @details Stores the OAuth2/OpenID Connect access token obtained from
       *          Keycloak authentication. This token is used for API authorization.
       */
      std::string m_accessToken;

      /**
       * @brief Default constructor
       * @details Initializes a new HelperObject instance. The access token
       *          is initialized as an empty string.
       */
      HelperObject();

      /**
       * @brief Destructor
       * @details Cleans up the HelperObject instance. Currently performs no
       *          special cleanup operations.
       */
      ~HelperObject();
    };

  } // namespace core
} // namespace logipad
