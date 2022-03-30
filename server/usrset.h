#ifndef USRSET_H
#define USRSET_H

#include <string>
#include <set>
#include <iostream>
#include <mutex>


struct Userset
{
    std::set<std::string> users;
    std::mutex usrmutex;
    
    Userset()
    {

    }

    ~Userset()
    {
        
    }
    void insert(const std::string& user)
    {
        std::unique_lock<std::mutex> lock(usrmutex);
        users.insert(user);
        std::cout << "insert successfully\n";
    }
    void erase(const std::string& user)
    {
        std::unique_lock<std::mutex> lock(usrmutex);
        users.erase(user);
    }

    bool find(const std::string& user)
    {
        std::lock_guard<std::mutex> lock(usrmutex);
        return users.find(user)==users.end()?false:true;
    }

};







#endif