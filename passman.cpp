#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <stdio.h>
#include <vector>
#include "sqlite3.h"

class Credentials {
    public:
        std::string websiteURL;
        std::string username;
        std::string password;
};

void executeAction(std::string selectedOption);
void addCredentials();
std::string genPass(int length = 16);

// static int callback();

int main()
{
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
        std::cout << "Option 1\n";
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

    sqlite3* db;
    int exit = 0;
    exit = sqlite3_open("credentials.db", &db);

    if (exit) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    std::string sql = "CREATE TABLE credentials("
                        "WEBSITE_URL TEXT NOT NULL, "
                        "USERNAME TEXT NOT NULL, "
                        "PASSWORD TEXT NOT NULL);";

    char* msgErr;
    exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &msgErr);

    if (exit != SQLITE_OK) {
        std::cerr << "Error creating table!" << std::endl;
        sqlite3_free(msgErr);
    } else {
        std::cout << "Table created successfully!" << std::endl;
    }

    // sqlite3_prepare_v2(db, sql.c_str(), NULL)

    std::cout << "Website U<dRL: ";
    std::cin >> newCredentials.websiteURL;
    
    std::cout << "Username/email: ";
    std::cin >> newCredentials.username;

    std::cout << "Password: ";
    std::cin >> newCredentials.password;

	/*
    sqlite3 *db;
    sqlite3_stmt * st;
    int id = 0;
    char *zErrMsg = 0;
    int rc;

    if (!std::filesystem::exists("credentials.db")) {
        std::cout << "credentials.db does not exist, creating...\n";
        
        rc = sqlite3_open("credentials.db", &db);
        
        if (rc) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            menu();
        } else {
            fprintf(stderr, "Opened database successfully\n");
        }

        std::string sql = "CREATE TABLE credentials("
                            "WEBSITE_URL TEXT NOT NULL, "
                            "USERNAME TEXT NOT NULL, "
                            "PASSWORD TEXT NOT NULL);";

        sqlite3_close(db);

        std::cout << "credentials.db successfully created.\n";
    }

    rc = sqlite3_open("credentials.db", &db);
    
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        menu();
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }
    */

	/*
    std::string sql = "INSERT INTO passwords (ID,title,password) VALUES (?,?,?)";

    if(sqlite3_open("pw.db", &db) == SQLITE_OK)
    {
        sqlite3_prepare( db, sql.c_str(), -1 &st, NULL);
        sqlite3_bind_text(st, 1, newCredentials.websiteURL.c_str(), newCredentials.websiteURL.length(), SQLITE_TRANSIENT);
        sqlite3_bind_text(st, 2, newCredentials.username.c_str(), newCredentials.username.length(), SQLITE_TRANSIENT);
        sqlite3_bind_text(st, 3, newCredentials.password.c_str(), newCredentials.password.length(), SQLITE_TRANSIENT);
        sqlite3_step( st );
    }  
    */

    sqlite3_close(db);

   std::cout << "Credentials added!\n";
   std::cout << newCredentials.websiteURL << std::endl << newCredentials.username << std::endl << newCredentials.password << std::endl;

   return;
}
/*
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}
*/

std::string genPass(int length)
{
	std::string passwd;
    const std::string letters{"abcdefghijklmnopqrstuvwxyz"};
    const std::string numbers{"123456789"};
    const std::string symbols{"!$%&/()=?^{[]}~@#+-"};

    const std::string allChars{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456789!$%&/()=?^{[]}~@#+-"};
    
    srand((unsigned) time(0));
    for (int i = 0; i < length; i++)
    {
        int randNum = rand() % allChars.length();
        passwd.append(1, allChars[randNum]);
    }

    return passwd;
}
