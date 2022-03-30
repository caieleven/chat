#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

int main()
{
    fstream file("users.txt");
    string line;
    while(getline(file, line))
    {
        istringstream sin(line);
        
    }

}



ifstream file("sss.csv", ios::in);
string line;
while(getline(file, line))
{
	istringstream sin(line);
    vector<int> temp;
    string tmp;
    while(getline(sin, tmp, ','))
        temp.push_back(stoi(tmp));
}