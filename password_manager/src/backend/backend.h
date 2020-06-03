#include <iostream>
#include <fstream>
#include <vector>

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
        void save_database();
        bool is_lower_case(char input_char);
        bool is_upper_case(char input_char);
        void encode(string string_to_encode, int rot);
        void decode(string string_to_decode, int rot);
        void parse_database(string decoded_string, string delimeter);
        bool is_found(string a_string, string contains);
        void show_credentials(int index);
};

void Backend::show_credentials(int index){
    int position = 0;
    string token;
    string credential_string = Backend::database_array[index];
    string delimiter = ":";

    while ((position = credential_string.find(delimiter)) != string::npos) {
        token = credential_string.substr(0, position);

        Backend::parsed_credentials.push_back(token);

        credential_string.erase(0, position + delimiter.length());
    }

    cout << 
    "Title: " << Backend::parsed_credentials[0] <<
    "Username: " << Backend::parsed_credentials[1] <<
    "Password: " << Backend::parsed_credentials[2] << endl;

    Backend::parsed_credentials = {};
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
        cout << Backend::encoded_database << endl;
        cout << Backend::decoded_database << endl;
        Backend::parse_database(Backend::decoded_database, ",");
        cout << Backend::database_array[1] << endl;
    }

    database_file.close();
}

void Backend::save_database(){
    //ofstream out_db_file ("backend/database.txt");

    try
	{
        cout << "Saving" << endl;

        for(int index = 0; index < 2; index++){
            cout << Backend::database_array[index] << endl;
        }

        cout << Backend::database_array[1] << endl;

		//out_db_file << Backend::encoded_database << endl;
		//out_db_file.close();

        cout << "Saving Comlete" << endl;
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

bool Backend::is_found(string a_string, string contains){
    return (a_string.find(contains) != string::npos) ? true: false;
};
