#include "passgen.h"

std::string genPass()
{
	std::string passwd;
    const std::string letters{"abcdefghijklmnopqrstuvwxyz"};
    const std::string numbers{"123456789"};
    const std::string symbols{"!$%&/()=?^{[]}~@#+-"};

    const std::string allChars{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456789!$%&/()=?^{[]}~@#+-"};
    
    srand((unsigned) time(0));
    for (int i = 0; i < 16; i++)
    {
        int randNum = rand() % allChars.length();
        passwd.append(1, allChars[randNum]);
    }

    return passwd;
}
