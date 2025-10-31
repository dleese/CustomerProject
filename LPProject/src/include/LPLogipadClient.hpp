/**
 * @file LPLogipadClient.hpp
 * @brief Logipad client class for Keycloak authentication
 *
 * Minimal client for connecting to Keycloak and obtaining access tokens.
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
 * @class LPLogipadClient
 * @brief Client for authenticating with Keycloak
 *
 * This class handles basic Keycloak authentication using password grant.
 */
class LPLogipadClient
{
public:
    std::string m_host;
    int m_port;
    std::string m_realm;
    std::string m_clientId;
    /**
     * @struct User
     * @brief Structure holding user information
     */
    struct User
    {
        std::string guid;
        std::optional<std::string> created_at;
        std::optional<std::string> created_by;
        std::optional<std::string> modified_at;
        std::optional<std::string> modified_by;
        std::optional<std::string> last_login_at;
        std::optional<std::string> last_activity_at;
        std::optional<std::string> last_document_service_activity;
        std::optional<std::string> last_eform_service_activity;
        std::optional<std::string> last_briefing_service_activity;
        std::optional<std::string> name;
        std::optional<std::string> type;
        std::optional<std::string> full_name;
        std::optional<std::string> email;
        std::optional<std::string> three_lc;
        std::optional<std::string> department;
        std::optional<std::string> description;
        bool is_active;
        bool is_reportable;
        // Convert to JSON for API requests
        nlohmann::json toJson() const;
    };

    /**
     * @struct Users
     * @brief Structure holding a list of users
     */
    struct Users
    {
        std::vector<User> users;
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
    LPLogipadClient(
        const std::string &host,
        int port = 443,
        const std::string &realm = "Logipad",
        const std::string &clientId = "lpclient",
        const std::string &username = "",
        const std::string &password = "");

    /**
     * @brief Destructor
     */
    ~LPLogipadClient();

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
     * @brief Get all users from the API
     * @param users Reference to Users struct to populate with the retrieved users
     * @param apiHost API hostname (default: "identity.demo.prod.logipad.net")
     * @param apiPort API port (default: 443)
     * @return true if request succeeded and users were retrieved, false otherwise
     */
    bool getAllUsers(Users &users, const std::string &m_host, int m_port);

private:
    std::string m_username;
    std::string m_password;
    std::string m_accessToken;

    std::unique_ptr<httplib::SSLClient> m_httplibClient;
    std::unique_ptr<LPKeyCloakClient> m_keycloakClient;
};
