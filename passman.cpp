#include <iostream>
#include <fstream>
#include "json.hpp" 
#include <filesystem>
#include <algorithm>

int main()
{
    if (!std::filesystem::exists("database.json")) {
        std::cout << "database.json does not exist, creating...\n";
        std::ofstream { "database.json" };
        std::cout << "database.json successfully created.\n";
    }

    std::cout << "---------- Password Manager ----------\nOptions:\n1. Get credentials\n2. Add credentials\n3. Remove credentials\n";
    int selectedOption;
    std::cout << "> ";
    std::cin >> selectedOption;

    switch(selectedOption) {
        case 1:
            // case 1
            break;
        case 2:
            std::string websiteURL;
            std::string username;
            std::string password;

            bool randomPassGen = false;

            std::cout << "Website URL: ";
            std::cin >> websiteURL;
            
            std::cout << "Username/email: ";
            std::cin >> username;

            std::cout << "Password: ";
            std::cin >> password;

            /*
            std::ifstream databaseFile("database.json");
            nlohmann::json data = nlohmann::json::parse(databaseFile);
            */
           
            /*
            if (data.contains(websiteURL) && data.at(websiteURL) == username) {
                std::string choice;
                std::cout << "This username/email already exists for this website, do you wish to overwrite the credentials? (y/N) ";
                std::cin >> choice;
                std::transform(choice.begin(), choice.end(), choice.begin(), ::tolower);
                if (choice == "y" || choice == "yes") {
                    data.at(websiteURL)
                }
            }
            */
    }
}