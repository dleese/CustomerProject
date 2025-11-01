/**
 * @file main.cpp
 * @brief Entry point of the LPProject application
 * @details This file contains the main entry point and the protected_main function
 *          that executes the application logic. It demonstrates usage of both
 *          LPKeyCloakClient and LPLogipadClient classes for Keycloak authentication
 *          and user management operations.
 * @author Dirk Leese
 * @date 2025
 *
 * @section Usage
 * The application performs the following operations:
 * 1. Authenticates with Keycloak using logipad::auth::KeycloakClient
 * 2. Creates a test user in Keycloak
 * 3. Authenticates with Keycloak using logipad::client::LogipadClient
 * 4. Retrieves all users from the Logipad identity service
 *
 * @note This is a demonstration/example application showcasing the client libraries.
 */

#include <iostream>
#include <LPHelperObject.hpp>
#include <LPLogipadClient.hpp>
#include <LPKeyCloakClient.hpp>
#include <Version.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp> // For JSON parsing

// Using declarations for cleaner code
using logipad::auth::KeycloakClient;
using logipad::client::LogipadClient;
using logipad::core::HelperObject;

/**
 * @brief Protected main function that executes application logic
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return Exit status code: 0 for success, non-zero for error
 * @details This function contains the main application logic separated from
 *          exception handling. It demonstrates:
 *          - Keycloak authentication using logipad::auth::KeycloakClient
 *          - User creation in Keycloak
 *          - Logipad client authentication using logipad::client::LogipadClient
 *          - User retrieval from Logipad identity service
 * @note Command-line arguments are currently unused but reserved for future functionality.
 */
int protected_main(int argc, char *argv[])
{
    (void)argc; // Reserved for future command-line argument processing
    (void)argv; // Reserved for future command-line argument processing

    // Example for Argument Processing
    // For now, we are not using any arguments, so we are not processing them.
    // if (argc < 2) {
    //    throw std::runtime_error("No input argument provided");
    //}
    // std::cout << "Program argument: " << argv[1] << std::endl;
    //
    // Define the realm
    const std::string &realm = "Logipad";

    KeycloakClient lpkcclient(
        "keycloak-cloud.logipad.net",
        443,
        "master",
        "admin-cli",
        "dd-admin",
        "xROv+Js$L2\\&RyCuexk$A5Kn" // if the password contains a backslash, it must be escaped!!
    );

    // Authenticate
    if (lpkcclient.authenticate())
    {

        std::cout << "Access Token: " << lpkcclient.getAccessToken() << std::endl;
        if (lpkcclient.createUser(KeycloakClient::UserInfo("aaaaa", "testuser@test.com", "Test", "User", "testpassword"), realm))
        {
            std::cout << "User created successfully" << std::endl;
        }
        else
        {
            std::cerr << "Failed to create user" << std::endl;
        }
    }
    else
    {
        std::cerr << "Failed to authenticate" << std::endl;
    }

    // Create the Logipad client
    LogipadClient client(
        "keycloak-cloud.logipad.net",
        443,
        "Logipad",
        "lpclient",
        "sysadm",
        "u2UkY4uBZk5uCscWCBpoh7nK");

    // Authenticate first
    if (client.authenticate())
    {
        // Get all users
        LogipadClient::Users users;
        if (client.getAllUsers(users, "identity.demo.prod.logipad.net", client.m_port))
        {
            std::cout << "Retrieved " << users.users.size() << " users" << std::endl;
            for (const auto &user : users.users)
            {
                std::cout << "User: " << user.guid << " ";
                if (user.name.has_value())
                {
                    std::cout << user.name.value() << " ";
                    if (user.email.has_value())
                    {
                        std::cout << " (" << user.email.value() << ")";
                    }
                }
                std::cout << std::endl;
            }
        }
        else
        {
            std::cerr << "Failed to retrieve users" << std::endl;
        }
    }
    return 0;
}

/**
 * @brief Main entry point of the application
 * @param argc The number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return Exit status code:
 *         - 0: Successful execution
 *         - 1: Standard exception caught
 *         - 2: Unknown exception caught
 * @details The main function provides centralized exception handling. It catches
 *          all exceptions, logs error messages, and returns appropriate exit codes.
 *          The actual application logic is executed in protected_main().
 * @note This separation allows for consistent error handling and logging across
 *       the entire application.
 */
int main(int argc, char *argv[])
{
    try
    {
        return protected_main(argc, argv);
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown error occurred." << std::endl;
        return 2;
    }
    return 0;
}