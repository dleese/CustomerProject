/**
 * @file LPLogipadClient.cpp
 * @brief Implementation of Logipad client class
 * @details This file contains the implementation of all LPLogipadClient methods,
 *          including authentication, user retrieval, and JSON serialization.
 * @author Dirk Leese
 * @date 2025
 */

#include <LPLogipadClient.hpp>
#include <iostream>

namespace logipad {
namespace client {

/**
 * @brief Constructor implementation
 * @details Initializes all connection parameters and creates the SSL client.
 */
LogipadClient::LogipadClient(
    const std::string &host,
    int port,
    const std::string &realm,
    const std::string &clientId,
    const std::string &username,
    const std::string &password) : m_host(host),
                                   m_port(port),
                                   m_realm(realm),
                                   m_clientId(clientId),
                                   m_username(username),
                                   m_password(password),
                                   m_httplibClient(std::make_unique<httplib::SSLClient>(host, port))
{
}

/**
 * @brief Destructor implementation
 * @details Automatically cleans up the SSL client and member variables.
 */
LogipadClient::~LogipadClient() = default;

/**
 * @brief Authenticate with Keycloak server
 * @details Performs password grant authentication and stores the access token.
 */
bool LogipadClient::authenticate()
{
    m_accessToken.clear();

    if (m_username.empty() || m_password.empty())
    {
        return false;
    }

    std::string token_url = "/realms/" + m_realm + "/protocol/openid-connect/token";

    // Prepare form data for password grant
    httplib::Params params;
    params.emplace("client_id", m_clientId);
    params.emplace("grant_type", "password");
    params.emplace("username", m_username);
    params.emplace("password", m_password);

    // Make the POST request
    auto res = m_httplibClient->Post(token_url.c_str(), params);

    // Check and parse the response
    if (res && res->status == 200)
    {
        auto json = nlohmann::json::parse(res->body);
        m_accessToken = json["access_token"];
        return true;
    }

    return false;
}

/**
 * @brief Convert User structure to JSON format
 * @details Serializes all user fields to JSON, including only optional fields that have values.
 */
nlohmann::json LogipadClient::User::toJson() const {
    nlohmann::json json;
    json["guid"] = guid;
    
    if (created_at.has_value()) json["created_at"] = created_at.value();
    if (created_by.has_value()) json["created_by"] = created_by.value();
    if (modified_at.has_value()) json["modified_at"] = modified_at.value();
    if (modified_by.has_value()) json["modified_by"] = modified_by.value();
    if (last_login_at.has_value()) json["last_login_at"] = last_login_at.value();
    if (last_activity_at.has_value()) json["last_activity_at"] = last_activity_at.value();
    if (last_document_service_activity.has_value()) json["last_document_service_activity"] = last_document_service_activity.value();
    if (last_eform_service_activity.has_value()) json["last_eform_service_activity"] = last_eform_service_activity.value();
    if (last_briefing_service_activity.has_value()) json["last_briefing_service_activity"] = last_briefing_service_activity.value();
    if (name.has_value()) json["name"] = name.value();
    if (type.has_value()) json["type"] = type.value();
    if (full_name.has_value()) json["full_name"] = full_name.value();
    if (email.has_value()) json["email"] = email.value();
    if (three_lc.has_value()) json["three_lc"] = three_lc.value();
    if (department.has_value()) json["department"] = department.value();
    if (description.has_value()) json["description"] = description.value();
    
    json["is_active"] = is_active;
    json["is_reportable"] = is_reportable;
    
    return json;
}

/**
 * @brief Retrieve all users from the Logipad identity API
 * @details Makes authenticated GET request, parses JSON response, and populates users vector.
 *          Handles both direct array responses and nested object responses.
 */
bool LogipadClient::getAllUsers(Users& users, const std::string& apiHost, int apiPort)
{
    // Clear existing users
    users.users.clear();

    // Check if authenticated
    if (m_accessToken.empty())
    {
        return false;
    }

    // Create SSL client for API host
    httplib::SSLClient apiClient(apiHost, apiPort);

    // Prepare headers with Bearer token
    std::string token = "Bearer " + m_accessToken;
    httplib::Headers headers = {
        { "Authorization", token },
        { "Accept", "application/json" }
    };

    // Make GET request to /users endpoint
    auto res = apiClient.Get("/users", headers);

    // Check and parse the response
    if (res && res->status == 200)
    {
        try
        {
            auto json = nlohmann::json::parse(res->body);
            
            // Check if response is an array
            if (json.is_array())
            {
                // Parse each user from the array
                for (const auto& userJson : json)
                {
                    User user;
                    
                    // Required field
                    if (userJson.contains("guid"))
                    {
                        user.guid = userJson["guid"].get<std::string>();
                    }
                    
                    // Optional fields
                    if (userJson.contains("created_at") && !userJson["created_at"].is_null())
                        user.created_at = userJson["created_at"].get<std::string>();
                    if (userJson.contains("created_by") && !userJson["created_by"].is_null())
                        user.created_by = userJson["created_by"].get<std::string>();
                    if (userJson.contains("modified_at") && !userJson["modified_at"].is_null())
                        user.modified_at = userJson["modified_at"].get<std::string>();
                    if (userJson.contains("modified_by") && !userJson["modified_by"].is_null())
                        user.modified_by = userJson["modified_by"].get<std::string>();
                    if (userJson.contains("last_login_at") && !userJson["last_login_at"].is_null())
                        user.last_login_at = userJson["last_login_at"].get<std::string>();
                    if (userJson.contains("last_activity_at") && !userJson["last_activity_at"].is_null())
                        user.last_activity_at = userJson["last_activity_at"].get<std::string>();
                    if (userJson.contains("last_document_service_activity") && !userJson["last_document_service_activity"].is_null())
                        user.last_document_service_activity = userJson["last_document_service_activity"].get<std::string>();
                    if (userJson.contains("last_eform_service_activity") && !userJson["last_eform_service_activity"].is_null())
                        user.last_eform_service_activity = userJson["last_eform_service_activity"].get<std::string>();
                    if (userJson.contains("last_briefing_service_activity") && !userJson["last_briefing_service_activity"].is_null())
                        user.last_briefing_service_activity = userJson["last_briefing_service_activity"].get<std::string>();
                    if (userJson.contains("name") && !userJson["name"].is_null())
                        user.name = userJson["name"].get<std::string>();
                    if (userJson.contains("type") && !userJson["type"].is_null())
                        user.type = userJson["type"].get<std::string>();
                    if (userJson.contains("full_name") && !userJson["full_name"].is_null())
                        user.full_name = userJson["full_name"].get<std::string>();
                    if (userJson.contains("email") && !userJson["email"].is_null())
                        user.email = userJson["email"].get<std::string>();
                    if (userJson.contains("three_lc") && !userJson["three_lc"].is_null())
                        user.three_lc = userJson["three_lc"].get<std::string>();
                    if (userJson.contains("department") && !userJson["department"].is_null())
                        user.department = userJson["department"].get<std::string>();
                    if (userJson.contains("description") && !userJson["description"].is_null())
                        user.description = userJson["description"].get<std::string>();
                    
                    // Boolean fields with defaults
                    user.is_active = userJson.contains("is_active") ? userJson["is_active"].get<bool>() : true;
                    user.is_reportable = userJson.contains("is_reportable") ? userJson["is_reportable"].get<bool>() : false;
                    
                    users.users.push_back(user);
                }
            }
            // Check if response is an object with a users array
            else if (json.is_object() && json.contains("users") && json["users"].is_array())
            {
                // Parse users from nested array
                for (const auto& userJson : json["users"])
                {
                    User user;
                    
                    if (userJson.contains("guid"))
                    {
                        user.guid = userJson["guid"].get<std::string>();
                    }
                    
                    // Optional fields (same as above)
                    if (userJson.contains("created_at") && !userJson["created_at"].is_null())
                        user.created_at = userJson["created_at"].get<std::string>();
                    if (userJson.contains("created_by") && !userJson["created_by"].is_null())
                        user.created_by = userJson["created_by"].get<std::string>();
                    if (userJson.contains("modified_at") && !userJson["modified_at"].is_null())
                        user.modified_at = userJson["modified_at"].get<std::string>();
                    if (userJson.contains("modified_by") && !userJson["modified_by"].is_null())
                        user.modified_by = userJson["modified_by"].get<std::string>();
                    if (userJson.contains("last_login_at") && !userJson["last_login_at"].is_null())
                        user.last_login_at = userJson["last_login_at"].get<std::string>();
                    if (userJson.contains("last_activity_at") && !userJson["last_activity_at"].is_null())
                        user.last_activity_at = userJson["last_activity_at"].get<std::string>();
                    if (userJson.contains("last_document_service_activity") && !userJson["last_document_service_activity"].is_null())
                        user.last_document_service_activity = userJson["last_document_service_activity"].get<std::string>();
                    if (userJson.contains("last_eform_service_activity") && !userJson["last_eform_service_activity"].is_null())
                        user.last_eform_service_activity = userJson["last_eform_service_activity"].get<std::string>();
                    if (userJson.contains("last_briefing_service_activity") && !userJson["last_briefing_service_activity"].is_null())
                        user.last_briefing_service_activity = userJson["last_briefing_service_activity"].get<std::string>();
                    if (userJson.contains("name") && !userJson["name"].is_null())
                        user.name = userJson["name"].get<std::string>();
                    if (userJson.contains("type") && !userJson["type"].is_null())
                        user.type = userJson["type"].get<std::string>();
                    if (userJson.contains("full_name") && !userJson["full_name"].is_null())
                        user.full_name = userJson["full_name"].get<std::string>();
                    if (userJson.contains("email") && !userJson["email"].is_null())
                        user.email = userJson["email"].get<std::string>();
                    if (userJson.contains("three_lc") && !userJson["three_lc"].is_null())
                        user.three_lc = userJson["three_lc"].get<std::string>();
                    if (userJson.contains("department") && !userJson["department"].is_null())
                        user.department = userJson["department"].get<std::string>();
                    if (userJson.contains("description") && !userJson["description"].is_null())
                        user.description = userJson["description"].get<std::string>();
                    
                    user.is_active = userJson.contains("is_active") ? userJson["is_active"].get<bool>() : true;
                    user.is_reportable = userJson.contains("is_reportable") ? userJson["is_reportable"].get<bool>() : false;
                    
                    users.users.push_back(user);
                }
            }
            
            return true;
        }
        catch (const nlohmann::json::exception& e)
        {
            // JSON parsing failed
            return false;
        }
    }

    return false;
}

} // namespace client
} // namespace logipad
