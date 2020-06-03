#include <iostream>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

class Id{
    public:
        string generate_salt(int len);
        bool first_time_check();
        void first_time_setup();
};

// Generates salt for the master password
string Id::generate_salt(int len){
    string salt = "";
    string charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUCWXYZ";
    int random_index;

    for (int i = 0; i < len; i++){
        // Generate random number from range 0 to 62 (charset length) - 1
        random_index = 0 + (rand() % static_cast<int>(charset.length() - 0 + 1));           
        salt += charset.substr(random_index, 1);
    }
    return salt;
};

// Checks if the program is run for the first time
bool Id::first_time_check(){
    ifstream guid_file("id/guid.txt");
    return (guid_file) ? false: true;
};

// Runs the first time setup
void Id::first_time_setup(){
    ofstream out_guid_file ("id/guid.txt");

    out_guid_file << Id::generate_salt(32) << endl;

    out_guid_file.close();
};