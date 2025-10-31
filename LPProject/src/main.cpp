/**
 * @file
 *
 * @brief Entry point of the application.
 *
 * This function initializes all subsystems, processes command line arguments,
 * and executes the main application logic.
 *
 * @return Exit status code, 0 for success or non-zero for error.
 *
 * @details
 * protected_main is called to execute the main logic.
 * It catches any exceptions and returns the appropriate exit status.
 *
 * Key Points
 * - Pass the command line parameters `argc` and `argv` from `main` into the protected function.
 * - Raise exceptions for error conditions (e.g., missing arguments).
 * - Catch exceptions centrally in `main()` for logging and return code handling.
 * - Keep `protected_main` focused on actual program logic.
 */

#include <iostream>
#include <LPHelperObject.hpp>
#include <LPLogipadClient.hpp>
#include <LPKeyCloakClient.hpp>
#include <Version.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp> // For JSON parsing

int protected_main(int argc, char *argv[])
{

    (void)argc; // for now, we are not using the arguments, so we are not processing them.
    (void)argv; // for now, we are not using the arguments, so we are not processing them.

    // Example for Argument Processing
    // For now, we are not using any arguments, so we are not processing them.
    // if (argc < 2) {
    //    throw std::runtime_error("No input argument provided");
    //}
    // std::cout << "Program argument: " << argv[1] << std::endl;
    //
    // Define the realm
    const std::string &realm = "Logipad";

    LPKeyCloakClient lpkcclient(
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
        if (lpkcclient.createUser(LPKeyCloakClient::UserInfo("aaaaa", "testuser@test.com", "Test", "User", "testpassword"), realm))
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
    LPLogipadClient client(
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
        LPLogipadClient::Users users;
        if (client.getAllUsers(users,"identity.demo.prod.logipad.net", client.m_port))
        {
            std::cout << "Retrieved " << users.users.size() << " users" << std::endl;
            for (const auto &user : users.users)
            {
                std::cout << "User: " << user.guid << " ";
                if (user.name.has_value())
                {
                    std::cout << user.name.value() << " ";
                    if (user.email.has_value()){
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
 * @brief Main function
 * @param argc The number of arguments
 * @param argv The arguments
 * @details The main function catches any exceptions and returns the appropriate exit status.
 * protected_main is called to execute the main logic.
 * @return The exit status
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