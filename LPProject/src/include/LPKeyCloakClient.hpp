/**
 * @file LPKeyCloakClient.hpp
 * @brief Header file for LPKeyCloakClient class
 * @details This file contains the declaration of the LPKeyCloakClient class,
 *          which provides a complete interface for interacting with Keycloak
 *          Admin REST API, including authentication and user management operations.
 * @author Dirk Leese
 * @date 2025
 * @copyright MIT License
 */

#pragma once

#include <string>
#include <memory>
#include <map>
#include <httplib.h>
#include <nlohmann/json.hpp>

/**
 * @namespace logipad::auth
 * @brief Authentication and Keycloak integration
 */

namespace logipad
{
    namespace auth
    {

        /**
         * @class KeycloakClient
         * @brief Client for interacting with Keycloak Admin REST API
         *
         * This class handles authentication and user management operations
         * against a Keycloak server.
         */
        class KeycloakClient
        {
        public:
            /**
             * @struct UserInfo
             * @brief Structure holding user information for Keycloak user creation
             * @details Contains all necessary fields to create a new user in Keycloak.
             *          All string fields are required except where noted.
             */
            struct UserInfo
            {
                std::string username;      ///< Unique username for the user (required)
                std::string email;         ///< User's email address (required)
                std::string firstName;     ///< User's first name (required)
                std::string lastName;      ///< User's last name (required)
                std::string password;      ///< Initial password for the user (required)
                bool enabled = true;       ///< Whether the user account is enabled (default: true)
                bool emailVerified = true; ///< Whether the email is verified (default: true)

                /**
                 * @brief Convert UserInfo to JSON format
                 * @return JSON object containing all user information formatted for Keycloak API
                 * @details Converts the UserInfo structure into a JSON object suitable for
                 *          Keycloak Admin REST API POST requests. The password is included in
                 *          a credentials array with type "password" and marked as temporary.
                 */
                nlohmann::json toJson() const;
            };

            /**
             * @brief Construct a new LPKeyCloakClient object
             * @param host Keycloak server hostname (e.g., "keycloak-cloud.logipad.net")
             * @param port Server port (default: 443 for HTTPS)
             * @param realm Keycloak realm name (e.g., "Logipad", "master")
             * @param clientId Client ID for authentication (e.g., "admin-cli", "lpclient")
             * @param username Username for authentication (admin user)
             * @param password Password for authentication
             * @details Creates a new Keycloak client instance and initializes the HTTPS client
             *          connection. Authentication must be performed separately using authenticate().
             * @note The SSL client is initialized with connection and read timeouts of 10 seconds.
             */
            KeycloakClient(
                const std::string &host = "keycloak-cloud.logipad.net",
                int port = 443,
                const std::string &realm = "",
                const std::string &clientId = "",
                const std::string &username = "",
                const std::string &password = "");

            /**
             * @brief Destructor
             */
            ~KeycloakClient();

            /**
             * @brief Authenticate with Keycloak and obtain access token
             * @return true if authentication succeeded and access token was obtained
             * @return false if authentication failed (check getLastError() for details)
             * @details Performs password grant authentication using the configured credentials.
             *          On success, stores the access token internally for use in subsequent API calls.
             * @note The access token is cleared before each authentication attempt.
             * @see getAccessToken()
             * @see getLastError()
             */
            bool authenticate();

            /**
             * @brief Create a new user in Keycloak
             * @param userInfo User information structure containing user details
             * @param realm Keycloak realm where the user should be created
             * @return true if user was created successfully (HTTP 201)
             * @return false if creation failed (check getLastError() for details)
             * @details Creates a new user in the specified Keycloak realm using the Admin REST API.
             *          The method automatically authenticates if no valid token is present.
             * @note Returns false with status 409 if a user with the same username already exists.
             * @warning Requires admin privileges in the specified realm.
             * @see authenticate()
             * @see getLastError()
             */
            bool createUser(const UserInfo &userInfo, const std::string &realm);

            /**
             * @brief Get the current access token
             * @return Access token string, empty if not authenticated
             */
            std::string getAccessToken() const { return m_accessToken; }

            /**
             * @brief Check if client is authenticated
             * @return true if authenticated, false otherwise
             */
            bool isAuthenticated() const { return !m_accessToken.empty(); }

            /**
             * @brief Get the last error message
             * @return Error message string
             */
            std::string getLastError() const { return m_lastError; }

            /**
             * @brief Set authentication credentials
             * @param username Username for authentication
             * @param password Password for authentication
             */
            void setCredentials(const std::string &username, const std::string &password);

        private:
            std::string m_host;
            int m_port;
            std::string m_realm;
            std::string m_clientId;
            std::string m_username;
            std::string m_password;
            std::string m_accessToken;
            std::string m_lastError;

            std::unique_ptr<httplib::SSLClient> m_client;

            /**
             * @brief Get authorization headers with Bearer token
             * @return Headers map containing Authorization header with Bearer token,
             *         Content-Type, and Accept headers
             * @details Constructs HTTP headers suitable for authenticated Keycloak API requests.
             *          If no access token is available, the Authorization header is omitted.
             * @note The Bearer token format is: "Bearer {access_token}"
             */
            httplib::Headers getAuthHeaders() const;

            /**
             * @brief Ensure client is authenticated, re-authenticate if necessary
             * @return true if client has a valid access token (or successfully authenticated)
             * @return false if authentication failed
             * @details Checks if an access token exists. If not, attempts to authenticate
             *          using the stored credentials. Used internally before making API calls.
             * @see authenticate()
             */
            bool ensureAuthenticated();
        };

    } // namespace auth
} // namespace logipad
