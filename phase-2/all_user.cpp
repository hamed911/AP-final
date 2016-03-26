#include"header.h"

void man:: signup(string command,ostringstream& out){
	vector<string> information;
	information = input_for_signup (command);
	if ( check_existence( information[1] ) )
		throw "You already signed up,please sign in with your account";
	user temp;
	temp.signup(information);
	people.push_back(temp);
	out << "You sucessfully signed up " << temp.username << " . Now you can sign in !" << endl;
}

void man:: signin(string command,ostringstream& out){
	vector<string> information;
	bool existence = false;
	information = get_two_input( command);
////////////////////////////////////////// check for 1 user
	for ( int i = 0; i < people.size() ; i++ )
		if( people[i].signin )
			throw "You have to sign out first!";
////////////////////////////////////////// check for 1 user
	for ( int i = 0; i < people.size() ; i++ )
		if(people[i].username == information[0] )
		if (  people[i].password != information[1] )
			throw "You enter wrong password,try again please!";
		else{
			people[i].signin = true;
			existence = true;
			current_user = &people[i];
			out << "welcome " << current_user->name << '!' << endl;
			break;
		}
	if ( !existence )
		throw "You have to sign up first!";
}

void man::signout(string command,ostringstream& out){
	check_no_more_input(command);
	if (current_user == NULL )
		throw"You have to sign in to use this command";
		for ( int i = 0; i < people.size() ; i++ )
			if(people[i].name == current_user->name ){
				people[i].signin = false;
				out<< "Bye " << current_user->name << endl;
				break;
			}
		current_user = NULL;
}

bool man:: handle_event (string command, ostringstream& out) {
string order;
stringstream ss;
ss << command ;
ss >> order;

	if( order == "signup" ){
		signup(command,out);
		return true;
	}
	else if ( order == "signin" ){
		signin( command,out);
		return true;
	}
	else if ( order == "signout" ){
		signout(command,out);
		return true;
	}
	return false;
}

bool man:: check_existence ( string id ){
	for( int i = 0; i < people.size() ; i++ )
		if ( id == people[i].username )
			return true;
	return false;
}
