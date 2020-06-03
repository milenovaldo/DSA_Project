#include <iostream>
#include <string>

using namespace std;

class Interface{
	public:
		// Initializes member functions and variables
		int selection;
		void ask_selection_message();
		void menu(bool access_grant);
		int get_selection();
};

// Selection menu
void Interface::ask_selection_message(){								
	{
		cout <<
		"Select an operation to run\n" <<
		"1. Add a new credential\n" <<
		"2. Modify a credential\n" <<
		"3. Delete a credential\n" << 
		"4. Search a credential\n" <<
		"5. Exit the program" <<
		endl;	
	}										
};

// Invokes the selection menu and asks for a selection input
void Interface::menu(bool access_grant){
	try{
		if (access_grant){
			Interface::ask_selection_message();
			cout << "Enter your selection: ";
			cin >> Interface::selection;
		}else{
			cout << "Access not granted." << endl;
		}
	}
	catch(const exception& e){
		cerr << e.what() << '\n';
	}
};

// Returns the selection
int Interface::get_selection(){
	return Interface::selection;
};

