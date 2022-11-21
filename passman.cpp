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
        bool exists;
};

void executeAction(std::string selectedOption);
void addCredentials();
void removeCredentials();
Credentials getCredentials(std::string websiteURL);
std::vector<std::string> getAllWebsites();
void listCredentials();
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
                    "5. List all credentials\n"
                    "Type \"exit\" to exit the program.\n";
        std::cout << "> ";
        std::cin >> selectedOption;
        if (selectedOption == "1" || selectedOption == "2" ||
            selectedOption == "3" || selectedOption == "4" ||
            selectedOption == "5") {

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
        if (!credentials.exists)
        {
            std::cout << "Website URL not found in database!" << std::endl;
            return;
        }
        std::cout << "Website URL: " << credentials.websiteURL << std::endl;
        std::cout << "Email/username: " << credentials.username << std::endl;
        std::cout << "Password: " << credentials.password << std::endl;
    } else if (selectedOption == "2")
    {
        addCredentials();
    } else if (selectedOption == "3")
    {
        removeCredentials();
    } else if (selectedOption == "4")
    {
        int inputLen;
        char genNewPass = 'y';
        std::cout << "Input password length: ";
        std::cin >> inputLen;

        std::cout << "hello" << std::endl;

        std::cout << genPass(inputLen) << std::endl;
        
        std::cout << "hello 2" << std::endl;

        while (std::tolower(genNewPass) == 'y') {
            std::cout << "hello 3" << std::endl;
            std::cout << "Generate new password? (y/n): ";
            std::cin >> genNewPass;
            std::cout << std::endl << "hello 4" << std::endl;

            if (std::tolower(genNewPass) == 'y')
            {
                std::cout << "hello 5" << std::endl;
                std::cout << genPass(inputLen) << std::endl;
                std::cout << "hello 6" << std::endl;
            }
        }
        return;

    } else if (selectedOption == "5") {
        listCredentials();
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

    std::cout << "add1" << std::endl;

    sqlExit = sqlite3_open("credentials.db", &db);

    if (sqlExit) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        exit(0);
    }

    std::cout << "add2" << std::endl;

    std::cout << "Website URL: ";
    std::cin >> newCredentials.websiteURL;
    
    std::cout << "Username/email: ";
    std::cin >> newCredentials.username;

    std::cout << "Password: ";
    std::cin >> newCredentials.password;

    sql = "INSERT INTO credentials VALUES (?,?,?)";

    std::cout << "add3" << std::endl;

    sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, newCredentials.websiteURL.c_str(), newCredentials.websiteURL.length(), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, newCredentials.username.c_str(), newCredentials.username.length(), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, newCredentials.password.c_str(), newCredentials.password.length(), SQLITE_STATIC);

    std::cout << "add4" << std::endl;

    sqlite3_step(stmt);

    std::cout << "add5" << std::endl;

    sqlite3_finalize(stmt);

    std::cout << "add6" << std::endl;

    std::cout << "Credentials added!\n";
    std::cout << newCredentials.websiteURL << std::endl << newCredentials.username << std::endl << newCredentials.password << std::endl;

    return;
}

void removeCredentials()
{
    bool websiteInDB;

    sqlite3_stmt* rem_stmt;

    std::cout << "rem1" << std::endl;

    std::string websiteURL;
    std::cout << "Website URL: ";
    std::cin >> websiteURL;

    sql = "SELECT * FROM credentials WHERE WEBSITE_URL = ?";

    sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);

    std::cout << "rem2" << std::endl;

    sqlite3_bind_text(stmt, 1, websiteURL.c_str(), websiteURL.length(), SQLITE_STATIC);

    int rows = sqlite3_step(stmt);

    std::cout << "rem3" << std::endl;

    if (rows == SQLITE_DONE)
    {
        websiteInDB = false;
    } else {
        websiteInDB = true;
    }

    std::cout << "rem4" << std::endl;

    if (websiteInDB)
    {
        std::cout << "rem5" << std::endl;
        while (true)
        {
            std::cout << "rem6" << std::endl;
            // std::cout << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)) << std::endl;;

            sql = "DELETE FROM credentials WHERE WEBSITE_URL = ?";

            sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &rem_stmt, nullptr);

            std::cout << "rem7" << std::endl;

            sqlite3_bind_text(rem_stmt, 1, websiteURL.c_str(), websiteURL.length(), SQLITE_STATIC);

            std::cout << "rem8" << std::endl;

            sqlite3_step(rem_stmt);

            std::cout << "rem9" << std::endl;

            if (rows == SQLITE_DONE) { std::cout << "hello 3" << std::endl; break; }
            
            rows = sqlite3_step(stmt);
        }
    }

    sqlite3_finalize(stmt);

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

    int rows = sqlite3_step(stmt);

    if (rows == SQLITE_DONE)
    {
        credentials.exists = false;
    } else {
        credentials.exists = true;
    }

    if (credentials.exists)
    {
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
    }

    sqlite3_finalize(stmt);

    return credentials;
}

std::vector<std::string> getAllWebsites()
{
    std::vector<std::string> websitesList;
    sql = "SELECT * FROM credentials";
    
    sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);

    int rows;

    while (true)
    {
        rows = sqlite3_step(stmt);

        if (rows == SQLITE_DONE) { break; }
        
        websitesList.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }

    sqlite3_finalize(stmt);

    return websitesList;
}

void listCredentials()
{
    std::vector<std::string> websitesList = getAllWebsites();

    std::cout << "---------- All Websites ----------" << std::endl;

    for (int i = 0; i < websitesList.size(); i++)
    {
        std::cout << websitesList[i] << std::endl;
    }

    return;
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
