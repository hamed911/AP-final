#include"header.h"

void trello::update_board_current_user(){
	for (int i=0 ; i< _board.size() ; i++ ){
	_board[i].members.current_user = all_user.current_user;
	}
}
/*
virtual void trello :: run() {
	

}
*/


void trello::read_from_file() {
	ifstream input;
	input.open("user");
	if ( !input.is_open() )
		return;
	string name,username,password;
	input >> name;
	while ( name != "$" ){
		input >> username >> password ;
		vector<string> information;
		information.push_back(name);
		information.push_back(username);
		information.push_back(password);
		user temp;
		temp.signup(information);
		all_user.people.push_back(temp);
		input>> name;
	}
	input.close();
	input.open("program");
	if ( !input.is_open() )
		return;
	input >> name;
	while(name !="end_of_program" ){
		if (name == "first_of_program")
			input >> board_counter;
		board temp_board;
		temp_board.read_from_file_board(input);
		input >> name;
		if (name == "&")
			break;
		_board.push_back(temp_board);
	}
	input.close();
}

void trello::write_to_file() {
	ofstream output;
	output.open("user");
	for(int i =0; i< all_user.people.size() ; i++ )
		output << all_user.people[i].name << ' ' <<all_user.people[i].username << ' ' << all_user.people[i].password << endl;
		output << "$" << endl;
	output.close();

	output.open( "program" );
	output << "first_of_program" << endl;
	output <<board_counter << endl;
	for (int i=0; i< _board.size() ; i++ )
		_board[i].write_to_file_board(output);
	output<<"end_of_program" << endl;
	output << "&" << endl;
	output.close();
}

void trello:: show_board(string command,ostringstream& out){
	if(!all_user.current_user)
			throw "You have to sign in first!";
		bool have_board = false;
		check_no_more_input(command);
		for(int i=0; i<_board.size() ; i++ )
			if( _board[i].show_board(out) )
				have_board = true;
		if( !have_board )
			throw "You have no board";
}

void trello:: enter_board(string command){
	if(!all_user.current_user)
			throw "You have to sign in first!";
		string b_number = get_one_input(command);
		int bord_num = atoi (b_number.c_str() );
		if ( bord_num == 0 )
			throw "You enter incorrect input! You should enter number of board";
		bool board_exist = false;
		for(int i=0; i<_board.size() ; i++ )
			if(bord_num == _board[i].board_number){
				bool user_of_board = false;
				for (int j=0; j< _board[i].members.people.size(); j++ )
					if ( _board[i].members.people[j].username == all_user.current_user->username )
						user_of_board = true;
				if (!user_of_board )
					throw "You are not member of this board,You can't enter to this board!";
				board_exist = true;
				current_board = &_board[i];
				break;
			}
		if ( !board_exist )
			throw "There isn't board with this number,so try again!";
		out << "You enter to board : " << current_board->board_name << " with number : " << current_board->board_number << endl;
}

void trello:: add_user(string command){
	if(!all_user.current_user)
			throw "You have to sign in first!";
		if(!current_board)
			throw "You have to enter to board first";
		if(all_user.current_user->username != current_board->maker )
			throw "You can't do this oprand,You are not admin!";
		
		string added_user = get_one_input(command);
		bool user_existence = false;
		for ( int i=0; i< all_user.people.size() ; i++ )
			if (added_user == all_user.people[i].username ){
				user_existence = true;
				for ( int j = 0; j< current_board->members.people.size() ; j++ )
					if(current_board->members.people[j].username == added_user )
						throw "This user existed in this board!";
				user temp = all_user.people[i];
				current_board->members.people.push_back(temp);
			}
			if(!user_existence)
				throw "This username doesn't exist!Try again!";
			out << "You added " << added_user <<
				" to " << current_board->board_name << " !" << endl;
}

void trello:: remove_user_from_board(string command){
	if(!all_user.current_user)
			throw "You have to sign in first!";
		if(!current_board)
			throw "You have to enter to board first";
		if(all_user.current_user->username != current_board->maker )
			throw "You can't do this oprand,You are not admin!";
		
		string removed_user = get_one_input(command);
		if(removed_user == current_board->maker )
			throw "You are admin of this board,you can't remove yourself!";
		bool user_existence = false;
		for ( int i=0; i< all_user.people.size() ; i++ )
			if (removed_user == all_user.people[i].username ){
				user_existence = true;
				all_user.people.erase(all_user.people.begin() + i );
			}
		if(!user_existence)
				throw "This username doesn't exist!Try again!";
		
		out << "You removed " << removed_user <<
			" from " << current_board->board_name << " !" << endl;
}

void trello:: create_board(string command){
	if(!all_user.current_user)
			throw "You have to sign in first!";
		string b_name = get_one_input ( command);
		user admin;
		admin = *all_user.current_user;
		board temp;
		temp.new_board(all_user.current_user->username,b_name,board_counter );
		temp.members.people.push_back(admin);
		_board.push_back(temp);
		out << "You created new board, and its name and number is <" << temp.board_name << "> , <" << temp.board_number <<'>' << endl;
		board_counter++;
}

void trello::remove_board(string command){
	if(!all_user.current_user)
			throw "You have to sign in first!";
	if(!current_board)
			throw "You have to enter to board first";
	if(all_user.current_user->username != current_board->maker )
		throw "You can't do this oprand,You are not admin!";
	string b_number = get_one_input (command);
	int bord_num = atoi (b_number.c_str() );
	if ( bord_num == 0 )
		throw "You enter incorrect input! You should enter number of board";
	bool existence = false;
	for ( int i =0 ; i<_board.size() ; i++ )
		if ( _board[i].board_number == bord_num ){
			_board.erase(_board.begin() +i );
			existence = true;
		}
	if(!existence)
		throw "There is no board with this number";
	out << "You remove board with number " << bord_num << " successfully!" << endl;
}

string trello::handle_program_event (string command) {
string order;
stringstream ss ;
ss << command;
ss >> order;

try {	
	if( order == "signup" || order == "signin" || order == "signout"){
		mutex.lock();
		if( all_user.handle_event(command,out) )
			update_board_current_user();
		mutex.unlock();
	}
	else {
		if(order == "show_boards"){
			mutex.lock();
			show_board(command,out);
			mutex.unlock();
		}
		else if(order == "enter_board"){
			mutex.lock();			
			enter_board(command);
			mutex.unlock();
		}
		else if(order == "add_user"){
			mutex.lock();
			add_user(command);
			mutex.unlock();
		}
		else if ( order == "remove_user_from_board"){
			mutex.lock();
			remove_user_from_board( command);
			mutex.unlock();
		}			
		else if ( order == "create_board"){
			mutex.lock();
			create_board(command);
			update_board_current_user();
			mutex.unlock();
		}
		else if (order == "remove_board"){
			mutex.lock();
			remove_board(command);
			mutex.unlock();
		}
		else if ( order == "exit" ){
			mutex.lock();
			out << "You exited!";
			mutex.unlock();
		}
		else if (order == "show_lists" ||order == "add_list" || order =="remove_list" || order =="move_card" || order =="add_card" ||
					order =="remove_card" || order =="rename_card" || order =="edit_card_description" || order =="edit_card_due_date" ||
					order =="assign_user" || order =="remove_user" || order =="comment" || order =="filter" || order =="show_cards")
					if(!current_board)
						throw "You can't use this command, you didn't enter to any board!";
					else {
						mutex.lock();
						if(current_board->handle_board_event( command,out) )
						mutex.unlock();
					}
		else{
			mutex.lock();
			out << "You enter wrong command,This command doesn't exist!Please try again!" << endl;
			mutex.unlock();
		}
	}
	}catch(char const* error){
		mutex.lock();
		out<< error << endl;
		mutex.unlock();
	} 
	string result = out.str();
	out.str("");
	return result;
}

