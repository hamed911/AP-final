#include"header.h"

void board:: read_from_file_board(istream& input){
	string list_,name,username,password;
	input >> list_;
	if (list_ == "no_list" )
		return;
	if (list_ != "first_of_board"){
		board_name = list_;
		input >> board_number >> maker >> list_counter >> card_counter ;
	}
	else
		input >> board_name >> board_number >> maker >> list_counter >> card_counter ;
	input >> name;
	while( name != "$"){
		input >> username >> password ;
		vector<string> information;
		information.push_back(name);
		information.push_back(username);
		information.push_back(password);
		user temp;
		temp.signup(information);
		members.people.push_back(temp);
		input>> name;

	}
	while(list_ != "end_of_board" ){
		list temp_list;
		if(temp_list.read_from_list(input))
			_list.push_back(temp_list);
		input >> list_;
	}

}
void board:: write_to_file_board(ostream& output){
	output << "first_of_board" << endl;
	output << board_name << ' ' << board_number << ' ' << maker << ' ' << list_counter << ' ' << card_counter << endl;
	for(int i =0; i<members.people.size() ;i++ )
		output << members.people[i].name << ' ' << members.people[i].username << ' ' << members.people[i].password << endl;
	output << '$' << endl;
	if ( _list.size() == 0 )
		output <<"no_list" << endl;
	for( int i = 0; i<_list.size() ; i++ )
		_list[i].write_to_list(output);
	output << "end_of_board" << endl;

}


bool board::show_board(){
	if(!members.current_user)
		throw "You have to sign in first!";
	for ( int i=0; i< members.people.size() ; i++)
		if(members.current_user->username == members.people[i].username ){
			cout << "The board name : " << board_name << " , The board number : " << board_number << endl ;
			return true;
		}
	return false;
}

void board::new_board(string admin,string b_name,int board_numb, int c_counter , int l_counter){
	card_counter = c_counter;
	list_counter = l_counter;
	board_number = board_numb;
	maker = admin;
	board_name = b_name;
}

void board::check_existence_security(){
	if (!members.current_user)
		throw "You have no access to this board!";
	bool exist = false;
	for(int i=0; i<members.people.size() ; i++ )
		if (members.current_user->username == members.people[i].username ){
			exist = true;
			break;
		}
		if(!exist)
			throw "You can't do this oprand,You deleted by admin!";
}

void board::show_list(){
	check_existence_security();
	check_no_more_input();
	if(_list.size()<1)
		throw "This board has no list!";
	for(int i =0; i< _list.size() ; i++ )
		_list[i].show_list();
}

void board::add_list(){
	check_existence_security();
	string name_new_list = get_one_input();
	for(int i=0; i<_list.size(); i++)
		if (name_new_list == _list[i].name)
			throw "You already create list with this number!";
	list temp;
	temp.create_list(name_new_list,list_counter);
	_list.push_back(temp);
	cout<<"You added list < " << name_new_list << "> with number " << list_counter << " to board : " << board_name << " !" << endl;
	list_counter++;
}

void board::remove_list(){
	check_existence_security();
	string list_numb = get_one_input();
	int remove_list = atoi (list_numb.c_str() );
	if ( remove_list == 0 )
		throw "You have to enter a number of list!";
	bool existence = false;
	for(int i = 0 ; i< _list.size() ; i++)
		if(_list[i].list_number == remove_list ){
			existence = true;
			_list.erase(_list.begin() + i);
		}
		if(!existence)
			throw "There is no list with this number";
	cout << "You remove list with number " << remove_list << " successfully!" << endl;
}

vector<card*> board::find_in_commen_card(vector<string> nam ){
	vector<card*> temp;
	for(int i =0; i<_list.size() ;i++ )
		for(int j=0; j<_list[i].cards.size() ; j++ ){
			vector<bool> existence;
			for( int k=0; k< _list[i].cards[j]->worker.size() ; k++)
				for (int m =0; m<nam.size() ; m++ )
					if(nam[m] == _list[i].cards[j]->worker[k].username)
						existence.push_back(true);
			if ( existence.size() == nam.size() ){
				card* finded_card = _list[i].cards[j];
				temp.push_back(finded_card);
			}

		}
	if( temp.size() < 1 )
		throw "There is no card for all this users!";
	else return temp;
}

list* board::find_list_of_card (int card_numb ){
	for (int i = 0 ; i<_list.size() ; i++ )
		for(int j=0; j< _list[i].cards.size() ; j++ )
			if(_list[i].cards[j]->card_number == card_numb )
				return &_list[i];

	return NULL;
}

bool board::add_card(string card_name, int list_numb){
	for(int i=0; i<_list.size() ;i++ )
		if(_list[i].list_number == list_numb ){
			card* new_card = new card();
			new_card->create_card(card_name,card_counter);
			card_counter++;
			_list[i].cards.push_back(new_card);
			return true;
		}
	return false;
}

void board::add_new_card(){
	check_existence_security();
	vector<string>information = get_two_input();
	int l_number = atoi (information[1].c_str() );
	if ( !l_number )
		throw "You have to enter the number of list correctly!";
	if( !add_card(information[0],l_number) )
		throw"There is no list with this number,try again!";
	cout<<"You added a new card with number : " << card_counter -1 << " , to list " << l_number <<" successfully!" << endl;
}

card* board::find_card_pointer(int card_num ){
	for (int i = 0 ; i<_list.size() ; i++ )
		for(int j=0; j< _list[i].cards.size() ; j++ )
			if(_list[i].cards[j]->card_number == card_num )
				return _list[i].cards[j];

	return NULL;
}

void board::rename_card(){
	check_existence_security();
	vector<string> information = get_two_input();
	int c_number = atoi (information[0].c_str() );
	if( !c_number )
		throw "You have to enter the number of card correctly!";
	card* edited;
	edited = find_card_pointer(c_number);
	if ( !edited )
		throw "There is no card with this number";
	edited-> name = information[1];
	cout<< "You changed name of card " << c_number << " to " << information[1] << " !" << endl;
}

void board::edit_card_description() {
	check_existence_security();
	vector<string> information = get_two_long_input();
	int c_number = atoi (information[0].c_str() );
	if( !c_number )
		throw "You have to enter the number of card correctly!";
	card* edited;
	edited = find_card_pointer(c_number);
	if ( !edited )
		throw "There is no card with this number";
	edited->description = information [1];
	cout<< "You changed description of card " << c_number << " to " << information[1] << " !" << endl;
}

void board::comment() {
	check_existence_security();
	vector<string> information = get_two_long_input();
	int c_number = atoi (information[0].c_str() );
	if( !c_number )
		throw "You have to enter the number of card correctly!";
	card* edited;
	edited = find_card_pointer(c_number);
	if ( !edited )
		throw "There is no card with this number";
	edited->statement.push_back(information[1]);
	cout<< "Your comment added to card " << c_number << " !" << endl;
}

bool board:: remove_card(int card_num ){
	check_existence_security();
	list* contain;
	contain = find_list_of_card(card_num);
	if( !contain )
		throw "There is no card with this number";
	for(int i = 0; i<contain->cards.size(); i++ )
		if(contain->cards[i]->card_number == card_num ){
			contain->cards.erase(contain->cards.begin() + i );
			return true;
		}
	return false;
}

void board::remove_special_card(){
	check_existence_security();
	string card_num = get_one_input();
	int c_number = atoi (card_num.c_str() );
	if (!c_number )
		throw "You have to enter a number of card";
	if( remove_card (c_number) )
		cout << "You removed card " << c_number << " successfully!"<< endl;
}

list* board::find_list( int list_num){
	for(int i =0; i<_list.size() ; i++ )
		if (_list[i].list_number == list_num)
			return &_list[i];
	return NULL;
}

void board::move_card(){
	check_existence_security();
	vector<string> information = get_two_input();
	int c_number = atoi (information[0].c_str() );
	if( !c_number )
		throw "You have to enter the number of card correctly!";
	int l_number = atoi (information[1].c_str() );
	if ( !l_number )
		throw "You have to enter the number of list correctly!";
	card* finded;
	finded = find_card_pointer(c_number);
	if(!finded)
		throw "There is no card with this number";
	list* destination;
	destination = find_list (l_number);
	if(!destination)
		throw "There is no list with this number";
	if( remove_card(c_number) ){
		destination->cards.push_back(finded);
		cout<< "You move card with number: " << c_number << " to list " << l_number << " successfully!" << endl;
	}
}

void board::assign_user(){
	check_existence_security();
	vector<string> information = get_two_input();
	int c_number = atoi (information[0].c_str() );
	if( !c_number )
		throw "You have to enter the number of card correctly!";
	card* edited;
	edited = find_card_pointer(c_number);
	if ( !edited )
		throw "There is no card with this number";
	bool user_existence = false;
	for ( int i=0; i< members.people.size() ; i++)
		if(members.people[i].username == information[1] ){
		user_existence = true;
		user temp;
		temp = members.people[i];
		edited->worker.push_back(temp);
		cout<< "You assigned user to card " << c_number << " !" << endl;
		}
		if(!user_existence)
			throw "This user didn't exist in this board!";
}

void board::remove_user() {
	check_existence_security();
	vector<string> information = get_two_input();
	int c_number = atoi (information[0].c_str() );
	if( !c_number )
		throw "You have to enter the number of card correctly!";
	card* edited;
	edited = find_card_pointer(c_number);
	if ( !edited )
		throw "There is no card with this number";
	bool user_existence = false;
	for ( int i=0; i< edited->worker.size() ; i++)
		if ( edited->worker[i].username == information[1] ){
			user_existence = true;
			edited->worker.erase(edited->worker.begin() + i );
			cout << "You removed "<< information[1] << " from card " << c_number << " !" << endl;
		}
	if ( !user_existence )
		throw "This user didn't exist !";
}

void board::edit_card_due_date(){
	check_existence_security();
	vector<string> information = get_two_input();
	int c_number = atoi (information[0].c_str() );
	if( !c_number )
		throw "You have to enter the number of card correctly!";
	string _date = check_date_input( information[1] );
	card* edited;
	edited = find_card_pointer(c_number);
	if ( !edited )
		throw "There is no card with this number";
	edited->date = _date;
	cout<< "You changed date of card " << c_number << " to " << _date << " !" << endl;
}

void board::filter(){
	check_existence_security();
	vector<string> information =get_filter_input();
	for(int j=0 ; j< information.size() ; j++ ){
		bool exist_user = false;
		for(int i = 0; i< members.people.size() ; i++ )
			if(information[j] == members.people[i].username ){
				exist_user = true;
				break;
			}
		if( !exist_user )
			throw "You searched users that they didn't exist in this board!";
	}
	vector<card*> finded_cards = find_in_commen_card(information);
	for(int i = 0; i< finded_cards.size() ; i++ )
		cout << finded_cards[i]->name << " : " << finded_cards[i]->card_number << endl;
}

void board::show_cards_of_list(){
	check_existence_security();
	string list_name = get_one_input();
	int list_nam = atoi (list_name.c_str() );
	if(list_nam == 0 )
		throw "You entered incorrect input,you have to insert number of list!";
	bool existence = false;
	for(int i =0; i<_list.size() ; i++ )
		if ( _list[i].list_number == list_nam ){
			existence = true;
			if(_list[i].cards.size() == 0 )
				throw"there is no card in this list";
			for(int j=0; j< _list[i].cards.size(); j++ )
				_list[i].cards[j]->show_card();
			break;
		}
		if (!existence )
			throw "This list didn't exist!try again";
}

bool board:: handle_board_event(string order){
	if( order == "show_lists" ){
		show_list();
		return true;
	}
	else if( order == "add_list"){
		add_list();
		return true;
	}
	else if( order == "remove_list"){
		remove_list();
		return true;
	}
	else if( order == "move_card"){
		move_card();
		return true;
	}
	else if( order == "add_card"){
		add_new_card();
		return true;
	}
	else if( order == "remove_card"){
		remove_special_card();
		return true;
	}
	else if( order == "rename_card"){
		rename_card();
		return true;
	}
	else if( order == "show_cards"){
		show_cards_of_list();
		return true;
	}
	else if( order == "edit_card_description"){
		edit_card_description();
		return true;
	}
	else if( order == "edit_card_due_date"){
		edit_card_due_date();
		return true;
	}
	else if( order == "assign_user"){
		assign_user();
		return true;
	}
	else if( order == "remove_user"){
		remove_user();
		return true;
	}
	else if( order == "comment"){
		comment();
		return true;
	}
	else if( order == "filter"){
		filter();
		return true;
	}

	return false;
}