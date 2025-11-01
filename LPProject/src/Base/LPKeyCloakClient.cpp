/**
 * MIT License
 *
 * Copyright (c) 2025 Dirk Leese
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **/

/**
 * @file LPKeyCloakClient.cpp
 * @brief Implementation of Keycloak client class
 * @details This file implements the LPKeyCloakClient class, which is used to interact with the Keycloak authentication server.
 */
#include <LPKeyCloakClient.hpp>
#include <iostream>
#include <stdexcept>

namespace logipad
{
    namespace auth
    {

        /**
         * @brief Convert UserInfo to JSON format for Keycloak API
         * @return JSON object containing user information
         * @details Serializes the UserInfo structure to a JSON format compatible with
         *          Keycloak Admin REST API. The password is included in a credentials array
         *          with type "password" and marked as temporary (user must change on first login).
         */
        nlohmann::json KeycloakClient::UserInfo::toJson() const
        {
            nlohmann::json json;
            json["username"] = username;
            json["email"] = email;
            json["firstName"] = firstName;
            json["lastName"] = lastName;
            json["enabled"] = enabled;
            json["emailVerified"] = emailVerified;

            if (!password.empty())
            {
                json["credentials"] = nlohmann::json::array();
                nlohmann::json cred;
                cred["type"] = "password";
                cred["value"] = "logipad";
                cred["temporary"] = true;
                json["credentials"].push_back(cred);
            }

            return json;
        }

        /**
         * @brief Constructor implementation
         * @details Initializes all member variables and creates the SSL client connection
         *          with 10-second connection and read timeouts.
         */
        KeycloakClient::KeycloakClient(
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
                                           m_client(std::make_unique<httplib::SSLClient>(host, port))
        {
            // Set connection timeout (10 seconds)
            m_client->set_connection_timeout(10, 0);
            m_client->set_read_timeout(10, 0);
        }

        /**
         * @brief Destructor implementation
         * @details Automatically cleans up the SSL client and all member variables.
         */
        KeycloakClient::~KeycloakClient() = default;

        /**
         * @brief Authenticate with Keycloak server
         * @details Performs password grant OAuth2 authentication and stores the access token.
         */
        bool KeycloakClient::authenticate()
        {
            m_lastError.clear();
            m_accessToken.clear();

            if (m_username.empty() || m_password.empty())
            {
                m_lastError = "Username or password not set";
                return false;
            }

            std::string tokenUrl = "/realms/" + m_realm + "/protocol/openid-connect/token";

            // Prepare form data for password grant
            httplib::Params params;
            params.emplace("client_id", m_clientId);
            params.emplace("grant_type", "password");
            params.emplace("username", m_username);
            params.emplace("password", m_password);

            // Make the POST request
            auto res = m_client->Post(tokenUrl.c_str(), params);

            if (res && res->status == 200)
            {
                try
                {
                    auto json = nlohmann::json::parse(res->body);
                    if (json.contains("access_token"))
                    {
                        m_accessToken = json["access_token"].get<std::string>();
                        return true;
                    }
                    else
                    {
                        m_lastError = "Access token not found in response";
                        return false;
                    }
                }
                catch (const nlohmann::json::exception &e)
                {
                    m_lastError = "Failed to parse JSON response: " + std::string(e.what());
                    return false;
                }
            }
            else
            {
                if (res)
                {
                    m_lastError = "Authentication failed with status: " + std::to_string(res->status);
                    if (!res->body.empty())
                    {
                        m_lastError += " - " + res->body;
                    }
                }
                else
                {
                    m_lastError = "Authentication request failed";
                }
                return false;
            }
        }

        // Ensure authenticated
        bool KeycloakClient::ensureAuthenticated()
        {
            if (m_accessToken.empty())
            {
                return authenticate();
            }
            return true;
        }

        // Get auth headers
        httplib::Headers KeycloakClient::getAuthHeaders() const
        {
            httplib::Headers headers;
            if (!m_accessToken.empty())
            {
                headers.emplace("Authorization", "Bearer " + m_accessToken);
            }
            headers.emplace("Content-Type", "application/json");
            headers.emplace("Accept", "application/json");
            return headers;
        }

        // Create user in Keycloak
        bool KeycloakClient::createUser(const UserInfo &userInfo, const std::string &realm)
        {
            m_lastError.clear();

            // Ensure we're authenticated
            if (!ensureAuthenticated())
            {
                m_lastError = "Not authenticated: " + m_lastError;
                return false;
            }

            // Validate user info
            if (userInfo.username.empty())
            {
                m_lastError = "Username is required";
                return false;
            }

            if (userInfo.email.empty())
            {
                m_lastError = "Email is required";
                return false;
            }

            // Build the API endpoint
            std::string userUrl = "/admin/realms/" + realm + "/users";

            // Convert user info to JSON
            nlohmann::json userJson = userInfo.toJson();
            std::string jsonBody = userJson.dump();

            // Get authentication headers
            auto headers = getAuthHeaders();

            // Make the POST request to create user
            auto res = m_client->Post(userUrl.c_str(), headers, jsonBody, "application/json");

            if (res && res->status == 201)
            {
                // Status 201 indicates user was created successfully
                return true;
            }
            else if (res && res->status == 409)
            {
                // Status 409 indicates user already exists
                m_lastError = "User with username '" + userInfo.username + "' already exists";
                return false;
            }
            else
            {
                if (res)
                {
                    m_lastError = "Failed to create user. Status: " + std::to_string(res->status);
                    if (!res->body.empty())
                    {
                        try
                        {
                            auto errorJson = nlohmann::json::parse(res->body);
                            if (errorJson.contains("errorMessage"))
                            {
                                m_lastError += " - " + errorJson["errorMessage"].get<std::string>();
                            }
                            else
                            {
                                m_lastError += " - " + res->body;
                            }
                        }
                        catch (...)
                        {
                            m_lastError += " - " + res->body;
                        }
                    }
                }
                else
                {
                    m_lastError = "Request failed to create user";
                }
                return false;
            }
        }

        // Set credentials
        void KeycloakClient::setCredentials(const std::string &username, const std::string &password)
        {
            m_username = username;
            m_password = password;
            // Clear existing token since credentials changed
            m_accessToken.clear();
        }

    } // namespace auth
} // namespace logipad
