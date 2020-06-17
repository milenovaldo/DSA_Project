//============================================================================
// Name        : password_manager.cpp
// Author      : Mileno Valdo, Sunny Jovita, Calvin Scorpiano
// Version     : 1.0
// Copyright   : Open source, baby
// Description : Password Manager in C++ that is kind of secure 
//				 but is free forever.
//============================================================================

#include <iostream>
#include <fstream>
#include "interface/interface.h"
#include "backend/backend.h"
#include "id/id.h"

using namespace std;

string mid_square(string input_unhashed){
    int ascii_total = 0;
    int total_squared = 0;
    string string_total_squared;
    int start;
    int length;

    for (char character : input_unhashed){
        ascii_total += (int)character;
    }

    total_squared = ascii_total * ascii_total;
    string_total_squared = to_string(total_squared);
    start = string_total_squared.length()/2 - 3;
    length = 7;

    return string_total_squared.substr(start, length);
};

string hash_input(string input_master_to_hash){
	fstream guid_file;
	string salt;
	string hashed_master;

	guid_file.open("id/guid.txt",ios::in);
	if (guid_file.is_open()){
		getline(guid_file, salt);
	}

	guid_file.close();
	hashed_master = mid_square(mid_square(salt) + input_master_to_hash);
	return hashed_master;
};

void create_master(string input_master_to_set){
	ofstream out_master_file ("id/master.txt");

	try
	{
		out_master_file << hash_input(input_master_to_set) << endl;
		out_master_file.close();
	}
	catch(const exception& e)
	{
		cerr << e.what() << '\n';
		cout << "Error. Delete guid.txt in the \"id\" subfolder and rerun the program." << endl;
	}

};

void route(int selection_input, Backend backend_obj){
	switch (selection_input)
	{
	case 1:
		/* Add Credential */
		backend_obj.load_database();
		backend_obj.add_credential();
		backend_obj.save_database();
		break;
	case 2:
		/* Modify Credential */
		backend_obj.load_database();
		backend_obj.modify_credential();
		backend_obj.save_database();
		break;
	case 3:
		/* Delete a credential */
		backend_obj.load_database();
		backend_obj.delete_credential();
		backend_obj.save_database();
		break;
	case 4: 
		/* Search a credential */
		backend_obj.load_database();
		backend_obj.search_credential();
		break;
	case 5:
		/* Exit program*/
		cout << "Exiting program" << endl;
		exit(0);
		break;

	default:
		break;
	}
};

int main(int argc, char *argv[]){
	string input_master;
	string input_new_master;
	Interface startup;
	Backend backend;
	Id session;
	
	// Welcome message 
	cout << "Welcome to our free password manager" << endl; 			
	
	// Check if it's the first time they ran the program and runs the first time setup
	if (session.first_time_check()){
		session.first_time_setup();
		cout << "Enter a new master password: ";
		cin >> input_new_master;
		create_master(input_new_master);
	}

	// Prompt to enter a master password and validate the master password
	while (1){
		cout << "Enter your master password: ";
		cin >> input_master;
		backend.set_master_hashed(hash_input(input_master));
		if (backend.verify_master(backend.get_master_hashed()) == 0){
			cout << "Incorrect master password. Try again." << endl;
		}else{
			while (backend.get_access()){
				startup.menu(backend.get_access());
				route(startup.get_selection(), backend);
			}
		}
	}

	return 0;
}
