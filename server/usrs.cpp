#include "usrs.h"

Usermap::Usermap()
{
    std::ifstream file("/home/caieleven/server/contacts/users.txt");
    std::string line, usrname, password;
    while (getline(file, line))
    {
        std::istringstream isline(line);
        isline >> usrname >> password;
        users.emplace(usrname, password);
    }
    file.close();
}

Usermap::~Usermap()
{
}

bool Usermap::isIn(const std::string &username, const std::string &password)
{
    std::lock_guard<std::mutex> lock(usrmutex);
    return users.find(username) != users.end() && users.at(username) == password;
}

void Usermap::registernew(const std::string &username, const std::string &password)
{
    std::unique_lock<std::mutex> lock(usrmutex);
    users.emplace(username, password);
    lock.unlock();
    std::cout << "insert successfully\n";
}


// int main()
// {
//     Usermap usrmap;
//     std::cout << usrmap.users.begin()->first;
//     for(auto iter = usrmap.users.begin(); iter != usrmap.users.end();  iter++)
//     {
//         std::cout << iter->first << "  " << iter->second << std::endl;
//     }
//     std::cout << usrmap.isIn("chen", "123") << std::endl; 
//     return 0;
// }