/**
 * @file LPLogipadClient.hpp
 * @brief Logipad client class for Keycloak authentication and user management
 * @details This file contains the declaration of the LPLogipadClient class,
 *          which provides functionality for authenticating with Keycloak and
 *          retrieving user information from the Logipad identity service.
 * @author Dirk Leese
 * @date 2025
 */

#pragma once

#include <string>
#include <memory>
#include <httplib.h>
#include <LPKeyCloakClient.hpp>
#include <optional>
#include <vector>
#include <nlohmann/json.hpp>

/**
 * @namespace logipad::client
 * @brief Logipad-specific client implementations
 */

namespace logipad
{
    namespace client
    {

        /**
         * @class LogipadClient
         * @brief Client for authenticating with Keycloak and managing Logipad users
         * @details This class handles Keycloak authentication using password grant and
         *          provides methods to retrieve user information from the Logipad identity service.
         *          It maintains connection settings and authentication state internally.
         */
        class LogipadClient
        {
        public:
            std::string m_host;     ///< Keycloak server hostname
            int m_port;             ///< Server port (typically 443 for HTTPS)
            std::string m_realm;    ///< Keycloak realm name
            std::string m_clientId; ///< Client ID for authentication

            /**
             * @struct User
             * @brief Structure holding Logipad user information
             * @details Represents a user in the Logipad system with all relevant metadata
             *          including timestamps, activity information, and user attributes.
             */
            struct User
            {
                std::string guid;                                          ///< Unique identifier (GUID) for the user (required)
                std::optional<std::string> created_at;                     ///< Timestamp when user was created
                std::optional<std::string> created_by;                     ///< User/entity that created this user
                std::optional<std::string> modified_at;                    ///< Timestamp when user was last modified
                std::optional<std::string> modified_by;                    ///< User/entity that last modified this user
                std::optional<std::string> last_login_at;                  ///< Timestamp of last login
                std::optional<std::string> last_activity_at;               ///< Timestamp of last activity
                std::optional<std::string> last_document_service_activity; ///< Last document service activity timestamp
                std::optional<std::string> last_eform_service_activity;    ///< Last eForm service activity timestamp
                std::optional<std::string> last_briefing_service_activity; ///< Last briefing service activity timestamp
                std::optional<std::string> name;                           ///< User's display name
                std::optional<std::string> type;                           ///< User type/role
                std::optional<std::string> full_name;                      ///< User's full name
                std::optional<std::string> email;                          ///< User's email address
                std::optional<std::string> three_lc;                       ///< Three-letter code (e.g., airline code)
                std::optional<std::string> department;                     ///< Department name
                std::optional<std::string> description;                    ///< User description
                bool is_active;                                            ///< Whether the user account is active
                bool is_reportable;                                        ///< Whether the user is reportable in analytics

                /**
                 * @brief Convert User to JSON format
                 * @return JSON object containing all user fields
                 * @details Serializes the User structure to JSON, including only fields that have values.
                 *          Optional fields are omitted if they don't have a value.
                 */
                nlohmann::json toJson() const;
            };

            /**
             * @struct Users
             * @brief Container structure holding a list of users
             * @details Wraps a vector of User objects for easy passing and manipulation.
             */
            struct Users
            {
                std::vector<User> users; ///< Vector containing all user records
            };

            /**
             * @brief Constructor
             * @param host Keycloak server hostname (e.g., "keycloak-cloud.logipad.net")
             * @param port Server port (default: 443 for HTTPS)
             * @param realm Keycloak realm name (e.g., "Logipad")
             * @param clientId Client ID for authentication (default: "lpclient")
             * @param username Username for authentication
             * @param password Password for authentication
             */
            LogipadClient(
                const std::string &host,
                int port = 443,
                const std::string &realm = "Logipad",
                const std::string &clientId = "lpclient",
                const std::string &username = "",
                const std::string &password = "");

            /**
             * @brief Destructor
             */
            ~LogipadClient();

            /**
             * @brief Authenticate and obtain access token
             * @return true if authentication succeeded, false otherwise
             */
            bool authenticate();

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
             * @brief Retrieve all users from the Logipad identity API
             * @param users Reference to Users struct to populate with retrieved users
             * @param apiHost API hostname (e.g., "identity.demo.prod.logipad.net")
             * @param apiPort API port (typically 443 for HTTPS)
             * @return true if request succeeded and users were retrieved successfully
             * @return false if request failed, not authenticated, or JSON parsing failed
             * @details Makes a GET request to the /users endpoint with Bearer token authentication.
             *          The users vector is cleared before populating with new data. Handles both
             *          array responses and object responses with nested "users" array.
             * @note Requires prior authentication using authenticate().
             * @warning The users parameter is cleared before population - existing data is lost.
             * @see authenticate()
             */
            bool getAllUsers(Users &users, const std::string &apiHost, int apiPort);

        private:
            std::string m_username;
            std::string m_password;
            std::string m_accessToken;

            std::unique_ptr<httplib::SSLClient> m_httplibClient;
            std::unique_ptr<auth::KeycloakClient> m_keycloakClient;
        };

    } // namespace client
} // namespace logipad
