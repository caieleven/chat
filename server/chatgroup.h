#ifndef CHATGROUP_H
#define CHARGROUP_H

#include <unordered_map>
#include <string>
#include <sstream>
#include <set>
#include <fstream>
#include <iostream>

class ChatGroup
{
public:
    std::unordered_map<std::string, std::set<std::string> > groups;
    ChatGroup();
    bool isIn(std::string& groupname, std::string& username)
    {
        return groups.at(groupname).find(username) != groups.at(groupname).end();
    }
};



#endif