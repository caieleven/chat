#ifndef USRS_H
#define USRS_H

#include <string>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <mutex>
#include <iostream>

/*
 * @TODO write new user information into users.txt


 */

class Usermap
{
public:
    std::unordered_map<std::string, std::string> users;
    std::mutex usrmutex;
    Usermap();

    ~Usermap();

    

    bool isIn(const std::string& username, const std::string& password);
    

    void registernew(const std::string& username, const std::string& password);


    // void erase(const std::string& user)
    // {
    //     std::lock_guard<std::mutex> lock(usrmutex);
        
    // }

};







#endif