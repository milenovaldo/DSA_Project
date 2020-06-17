#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class Backend{
    // Initializes variables and member functions
    private:
        string master_password_hash;
        bool access_granted = false;
        string line;
        string encoded_database;
        string decoded_database;
        vector<string> database_array;
        vector<string> parsed_credentials;
    public:
        void set_master_hashed(string master_input_hashed);
        string get_master_hashed();
        bool verify_master(string master_hashed);
        bool get_access();
        void load_database();
        void add_credential();
        void modify_credential();
        void delete_credential();
        void save_database();
        void search_credential();
        bool is_lower_case(char input_char);
        bool is_upper_case(char input_char);
        void encode(string string_to_encode, int rot);
        void decode(string string_to_decode, int rot);
        void parse_database(string decoded_string, string delimeter);
        void parse_credential(string credential, string delimeter);
        bool is_found(string a_string, string contains);
        int search(string title);
};

// Assigns the hashed master password
void Backend::set_master_hashed(string master_input_hashed){
    Backend::master_password_hash = master_input_hashed;
};

// Returns the hashed master password
string Backend::get_master_hashed(){
    return Backend::master_password_hash;
};

// Checks if the hashed input master password matches the stored hashed master password
bool Backend::verify_master(string salt_master_hashed){
    fstream master_file;

    master_file.open("id/master.txt",ios::in);

    if (master_file.is_open()){
        getline(master_file, Backend::master_password_hash);
        master_file.close();
    }

    Backend::access_granted = true;

    return (salt_master_hashed == Backend::get_master_hashed()) ? true: false;
};

// Returns access grant
bool Backend::get_access(){
    return Backend::access_granted;
};

// Loads the database.txt file which contains the encoded data
void Backend::load_database(){
    fstream database_file;

    database_file.open("backend/database.txt");

    if(database_file.is_open()){
        while (getline(database_file, Backend::line)){
            Backend::encoded_database += Backend::line;
        }
        Backend::decode(Backend::encoded_database, 13);
        Backend::parse_database(Backend::decoded_database, ",");
    }

    database_file.close();
}

// Add a new credential
void Backend::add_credential(){
    string title;
    string username;
    string password;

    cout << "Enter a title: ";
    cin >> title;

    cout << "Enter a username: ";
    cin >> username;

    cout << "Enter a password: ";
    cin >> password;

    string string_to_append = title + ":" + username + ":" + password + ":";
    Backend::database_array.push_back(string_to_append);
};

// Modify a credential
void Backend::modify_credential(){
    string title_input;
    string username;
    string password;

    cout << "Enter title to modify (Case Sensitive): ";
    cin >> title_input;

    cout << "Title found at position: " << Backend::search(title_input) << endl;

    if(Backend::search(title_input) == -1){
        cout << "Title not found. Try again." << endl;
        return;
    }else{
        cout << "Enter a new username: ";
        cin >> username;

        cout << "Enter a new password: ";
        cin >> password;

        Backend::database_array[Backend::search(title_input)] = title_input + ":" + username + ":" + password + ":";
    }
};

// Delete a credential by providing a title
void Backend::delete_credential(){
    string title_input;

    cout << "Enter title to delete (Case Sensitive): ";
    cin >> title_input;

    // erase element
    if(Backend::search(title_input) == -1){
        cout << "Title not found. Try again." << endl;
        return;
    }else{
        int new_array_size = Backend::database_array.size() - 1;

        int start_position = Backend::search(title_input);

        for(int position = start_position; position < new_array_size; position++){
            Backend::database_array[position] = Backend::database_array[position + 1];
        }

        Backend::database_array.pop_back(); // Removes last element of the array.
    }

};

// Search for a credential by providing a title
void Backend::search_credential(){
    string title_input, title, username, password, credential_to_parse;
    
    cout << "Enter a title to search (Case Sensitive): ";
    cin >> title_input;

    credential_to_parse =  Backend::database_array[search(title_input)];

    if(Backend::search(title_input) == -1){
        cout << "Title not found. Try again." << endl;
        return;
    }else{
        Backend::parse_credential(credential_to_parse, ":");

        title = Backend::parsed_credentials[0];
        username = Backend::parsed_credentials[1];
        password = Backend::parsed_credentials[2];

        cout << "Title: " << title << endl;
        cout << "Username: " << username << endl;
        cout << "Password: " << password << endl;
    }
};

// Saves the database_array into the database.txt file
void Backend::save_database(){
    ofstream out_db_file ("backend/database.txt");

    try
	{
        cout << "Saving" << endl;

        Backend::decoded_database = "";

        for(int i = 0; i < Backend::database_array.size(); i++){
            Backend::decoded_database += Backend::database_array[i] + ",";
        }

        Backend::encode(Backend::decoded_database, 13);

		out_db_file << Backend::encoded_database << endl;
		out_db_file.close();

        cout << "Saving Complete" << endl;
	}
	catch(const exception& e)
	{
		cerr << e.what() << '\n';
		cout << "Error. " << endl;
	}

};

// Checks if the arguement character is lower case
bool Backend::is_lower_case(char input_char){
    return (input_char >= 'a' && input_char <= 'z');
};

// Checks if the arguement character is upper case
bool Backend::is_upper_case(char input_char){
    return (input_char >= 'A' && input_char <= 'Z');
};

// Encodes the arguement string using rot-n encoding
void Backend::encode(string string_to_encode, int rot){
    Backend::encoded_database = "";
    for (char character : string_to_encode){
        if (Backend::is_upper_case(character))
            Backend::encoded_database += ((int)character - (int)'A' + rot) % 26 + 'A';
        else if (Backend::is_lower_case(character))
            Backend::encoded_database += ((int)character - (int)'a' + rot) % 26 + 'a';
        else
            Backend::encoded_database += character;
    }
};

// Decodes the arguement string using rot-n encoding
void Backend::decode(string string_to_decode, int rot){
    Backend::decoded_database = "";
    for (char character : string_to_decode){
        if (Backend::is_upper_case(character))
            Backend::decoded_database += ((int)character - (int)'A' - rot + 26) % 26 + 'A';
        else if (Backend::is_lower_case(character))
            Backend::decoded_database += ((int)character - (int)'a' - rot + 26) % 26 + 'a';
        else
            Backend::decoded_database += character;
    }
};

// Parse the arguement in_string into an array
void Backend::parse_database(string in_string, string delimiter){
    int position = 0;
    string token;
    while ((position = in_string.find(delimiter)) != string::npos) {
        token = in_string.substr(0, position);

        Backend::database_array.push_back(token);

        in_string.erase(0, position + delimiter.length());
    }
};

// Parses the selected credential to be displayed
void Backend::parse_credential(string credential, string delimiter){
    int position = 0;

    Backend::parsed_credentials.clear();

    string token;
    while ((position = credential.find(delimiter)) != string::npos) {
        token = credential.substr(0, position);

        Backend::parsed_credentials.push_back(token);

        credential.erase(0, position + delimiter.length());
    }
};

// Binary search algorithm 
int Backend::search(string title){
    // Sorting the array of strings in alphabetical order.
    sort(Backend::database_array.begin(), Backend::database_array.end());

    int lower = 0;
    int upper = Backend::database_array.size() - 1;
    while (lower <= upper) {
        int mid = lower + (upper - lower) / 2;
        int res;
        if ((Backend::is_found(Backend::database_array[mid], title))){
            res = 0;
        }
        if (res == 0)
            return mid;
        if (title > (Backend::database_array[mid]))
            lower = mid + 1;
        else
            upper = mid - 1;
    }
    return -1;

};

bool Backend::is_found(string a_string, string contains){
    return (a_string.find(contains) != string::npos) ? true: false;
};
