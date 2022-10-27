#include <iostream>
#include <fstream>
#include <filesystem>
#include <stdio.h>
#include <vector>
#include <sqlite3.h>

class Credentials {
    public:
        std::string websiteURL;
        std::string username;
        std::string password;
};

void executeAction(std::string selectedOption);
void addCredentials();
Credentials getCredentials(std::string websiteURL);
std::string genPass(int length = 16);

sqlite3* db;
sqlite3_stmt* stmt;
char* msgErr;
std::string sql;
int sqlExit = 0;

int main()
{
    sqlExit = sqlite3_open("credentials.db", &db);

    if (sqlExit) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        exit(0);
    }

    sql = "CREATE TABLE IF NOT EXISTS credentials("
            "WEBSITE_URL TEXT NOT NULL, "
            "USERNAME TEXT NOT NULL, "
            "PASSWORD TEXT NOT NULL);";

    sqlExit = sqlite3_exec(db, sql.c_str(), NULL, 0, &msgErr);

    if (sqlExit != SQLITE_OK) {
        std::cerr << "Error creating table!" << std::endl;
        sqlite3_free(msgErr);
    } else {
        std::cout << "Table created successfully or already existed!" << std::endl;
    }

    std::string selectedOption;
    while (true) {
        std::cout << "---------- Password Manager ----------\nOptions:\n1. Get credentials\n"
                    "2. Add credentials\n3. Remove credentials\n4. Generate password\n"
                    "Type \"exit\" to exit   the program.\n";
        std::cout << "> ";
        std::cin >> selectedOption;
        if (selectedOption == "1" || selectedOption == "2" ||
            selectedOption == "3" || selectedOption == "4") {

            std::cout << "selected option is good" << std::endl;
        
        } else if (selectedOption == "exit")
        {
            exit(0);
        } else {
			std::cout << "selected option is invalid" << std::endl;
		}
		executeAction(selectedOption);
    }
}

void executeAction(std::string selectedOption)
{
    if (selectedOption == "1")
    {
        std::string websiteURL;
        std::cout << "Input website to retrieve credentials for: ";
        std::cin >> websiteURL;
        Credentials credentials = getCredentials(websiteURL);
        std::cout << "Website URL: " << credentials.websiteURL << std::endl;
        std::cout << "Email/username: " << credentials.username << std::endl;
        std::cout << "Password: " << credentials.password << std::endl;
    } else if (selectedOption == "2")
    {
        addCredentials();
    } else if (selectedOption == "3")
    {
        std::cout << "Option 3\n";
    } else if (selectedOption == "4")
    {
        int inputLen;
        char genNewPass = 'y';
        std::cout << "Input password length: ";
        std::cin >> inputLen;

        std::cout << genPass(inputLen) << std::endl;
        
        while (std::tolower(genNewPass) == 'y') {
            std::cout << "Generate new password? (y/N): ";
            std::cin >> genNewPass;

            if (std::tolower(genNewPass) == 'y')
            {
                std::cout << genPass(inputLen) << std::endl;
            }
        }
        return;

    } else
    {
        std::cout << "Invalid input";
    }    
    return;
}

void addCredentials()
{
    Credentials newCredentials;

    bool randomPassGen = false;

    sqlExit = sqlite3_open("credentials.db", &db);

    if (sqlExit) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        exit(0);
    }

    std::cout << "Website URL: ";
    std::cin >> newCredentials.websiteURL;
    
    std::cout << "Username/email: ";
    std::cin >> newCredentials.username;

    std::cout << "Password: ";
    std::cin >> newCredentials.password;

    sql = "INSERT INTO credentials VALUES (?,?,?)";

    sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, newCredentials.websiteURL.c_str(), newCredentials.websiteURL.length(), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, newCredentials.username.c_str(), newCredentials.username.length(), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, newCredentials.password.c_str(), newCredentials.password.length(), SQLITE_STATIC);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    sqlite3_close(db);

    std::cout << "Credentials added!\n";
    std::cout << newCredentials.websiteURL << std::endl << newCredentials.username << std::endl << newCredentials.password << std::endl;

    return;
}

Credentials getCredentials(std::string websiteURL)
{
    sqlExit = sqlite3_open("credentials.db", &db);

    if (sqlExit) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        exit(0);
    }
    
    Credentials credentials;
    credentials.websiteURL = websiteURL;

    sql = "SELECT * FROM credentials WHERE WEBSITE_URL = ?";

    sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, websiteURL.c_str(), websiteURL.length(), SQLITE_STATIC);

    sqlite3_step(stmt);

    for (int i = 0; i < sqlite3_column_count(stmt); i++)
    {
        if (i == 1)
        {
            credentials.username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
        } else if (i == 2)
        {
            credentials.password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
        }
    }

    sqlite3_finalize(stmt);

    return credentials;
}

std::string genPass(int length)
{
	std::string password;
    const std::string letters{"abcdefghijklmnopqrstuvwxyz"};
    const std::string numbers{"123456789"};
    const std::string symbols{"!$%&/()=?^{[]}~@#+-"};

    const std::string allChars{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456789!$%&/()=?^{[]}~@#+-"};
    
    srand((unsigned) time(0));
    for (int i = 0; i < length; i++)
    {
        int randNum = rand() % allChars.length();
        password.append(1, allChars[randNum]);
    }

    return password;
}
