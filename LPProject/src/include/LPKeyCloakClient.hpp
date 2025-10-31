/**
 * @file LPKeyCloakClient.hpp
 * @brief Header file for LPKeyCloakClient class
 * @details This file contains the declaration of the LPKeyCloakClient class.
 */

#pragma once

#include <string>
#include <memory>
#include <map>
#include <httplib.h>
#include <nlohmann/json.hpp>

/**
 * @class LPKeyCloakClient
 * @brief Client for interacting with Keycloak Admin REST API
 *
 * This class handles authentication and user management operations
 * against a Keycloak server.
 */
class LPKeyCloakClient
{
public:
    /**
     * @struct UserInfo
     * @brief Structure holding user information for creation
     */
    struct UserInfo
    {
        std::string username;
        std::string email;
        std::string firstName;
        std::string lastName;
        std::string password;
        bool enabled = true;
        bool emailVerified = true;

        // Convert to JSON for API requests
        nlohmann::json toJson() const;
    };

    /**
     * @brief Constructor
     * @param host Keycloak server hostname (e.g., "keycloak-cloud.logipad.net")
     * @param port Server port (default: 443 for HTTPS)
     * @param realm Keycloak realm name (e.g., "Logipad")
     * @param clientId Client ID for authentication
     * @param username Username for authentication
     * @param password Password for authentication
     */
    LPKeyCloakClient(
        const std::string &host = "keycloak-cloud.logipad.net",
        int port = 443,
        const std::string &realm = "",
        const std::string &clientId = "",
        const std::string &username = "",
        const std::string &password = "");

    /**
     * @brief Destructor
     */
    ~LPKeyCloakClient();

    /**
     * @brief Authenticate and obtain access token
     * @return true if authentication succeeded, false otherwise
     */
    bool authenticate();

    /**
     * @brief Create a new user in Keycloak
     * @param userInfo User information to create
     * @return true if user was created successfully, false otherwise
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
     * @return Headers map with Authorization header
     */
    httplib::Headers getAuthHeaders() const;

    /**
     * @brief Refresh access token if needed
     * @return true if token is valid, false otherwise
     */
    bool ensureAuthenticated();
};
