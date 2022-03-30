#include "chatgroup.h"

ChatGroup::ChatGroup()
{
    std::ifstream file("/home/caieleven/server/contacts/group.txt");
    std::string line, username, groupname;
    std::set<std::string> tempset;
    while (getline(file, line))
    {
        std::istringstream isline(line);
        isline >> groupname;
        while(isline >> username)
        {
            tempset.emplace(username);
        }
        groups.emplace(groupname, tempset);
    }
    file.close();
}

// #include <iostream>

// int main()
// {
//     ChatGroup ch;
//     for(auto x:ch.groups)
//         std::cout << x.first << std::endl;
//     return 0;

// }