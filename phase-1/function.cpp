#include"header.h"

vector<string> input_for_signup () {
	string line;
	vector<string> input;
	getline(cin, line);
	
	line.erase( line.begin() );
	char * cstr = new char [line.length()+1];
	strcpy (cstr, line.c_str());
	char * p = strtok (cstr," ");
	while (p!=0)
	{
		string temp = p;
		input.push_back(temp);
		p = strtok(NULL," ");
	}
  delete[] cstr;
  // get full name
  if ( input.size() == 3 )
	  return input;
  else if (input.size() < 3 )
	  throw "You enter incorrect input!\nUse this structure : signup name username password";
  else {
	  for ( int i=1; i< input.size()-2 ; i++ )
		  input[0] += ' ' + input[i]; 
		vector<string> temp;
		temp.push_back(input[0]);
		temp.push_back(input[input.size()-2]);
		temp.push_back(input[input.size()-1]);
	return temp;
  }
}

vector<string> get_two_input(){
	string line;
	vector<string> input;
	getline(cin, line);

	line.erase( line.begin() );
	char * cstr = new char [line.length()+1];
	strcpy (cstr, line.c_str());
	char * p = strtok (cstr," ");
	while (p!=0)
	{
		string temp = p;
		input.push_back(temp);
		p = strtok(NULL," ");
	}
	delete[] cstr;
  // get full name
  if ( input.size() == 2 )
	  return input;
  else
	  throw "You enter incorrect input!\nYou have to use this command with 2 seperated word ( or number ) by space";

}

vector<string> get_two_long_input() {
	string line;
	vector<string> input;
	getline(cin, line);
	if(line == "" )
		throw "You enter incorrect input!\nYou have to use more than one words for this command";
	char * cstr = new char [line.length()+1];
	strcpy (cstr, line.c_str());
	char * p = strtok (cstr," ");
	string temp = p;
	input.push_back(temp);
	p = strtok(NULL,"\0");
	temp = p;
	input.push_back(temp);
	delete[] cstr;
	return input;
}

vector<string> get_filter_input(){
	string line;
	vector<string> input;
	getline(cin, line);
	if(line == "")
		throw "You entered wrong input!This command needs at least one user name!";
	line.erase( line.begin() );
	char * cstr = new char [line.length()+1];
	strcpy (cstr, line.c_str());
	char * p = strtok (cstr," ");
	while (p!=0)
	{
		string temp = p;
		input.push_back(temp);
		p = strtok(NULL," ");
	}
	delete[] cstr;
	return input;
}

string get_one_input(){
	string line;
	vector<string> input;
	getline(cin, line);
	if(line == "" )
		throw "You enter incorrect input!\nThis command needs just one input";
	line.erase( line.begin() );
	for(int i = 0 ; i< line.size() ; i++)
		if (line[i] == ' ')
			throw "This command need just one input";
	return line;
}

void check_no_more_input() {
	string line;
	getline(cin , line );
	if(line != "" )
		throw "You enter incorrect input!\nThis command doesn't need any extra word";
}

string check_date_input(string date){
	if( date.size() != 10 || date[4] !='-' || date[7] !='-' )
		throw "You enter a wrong date,use this structure:\nYYYY-MM-DD";
	for(int i=0; i<date.size() ; i++ )
		if( date[i]< '0' || date[i]>'9' )
			if(i == 4 || i == 7)
				continue;
			else 
				throw "You enter a wrong date,use this structure:\nYYYY-MM-DD";
	return date;
}

/////////////////////////////////////////////////////card
////check deleted person from board
void card:: rename_card ( string new_name){
	name = new_name;
}
void card :: edit_card_description(string descript){
	description = descript;
}
void card::edit_due_date(string d ){
	date = d;
}
void card::assign_user( user new_worker) {
	worker.push_back(new_worker);
}
bool card::remove_user(string username){
	for (int i = 0 ; i< worker.size() ; i++)
		if (worker[i].username == username){
			worker.erase(worker.begin() + i );
			return true;
		}
	return false;		
}
void card::comment( string cm ) {
	statement.push_back(cm);
}
void card::show_card() {
	cout<< "card name: " << name << " , and card number: " << card_number << endl;
	cout<< "user for this card: " ;
	if(worker.size() == 0 )
		cout<< "There is no user!";
	else
		for ( int i =0; i< worker.size() ; i++ )
			cout<< worker[i].username << " ,";
	cout<< endl;
	cout<<"card description: " << description << endl;
	cout << "card due date is: " << date << endl;
	cout << "camment : " << endl;
	if ( statement.size() == 0 )
		throw "There is no comment!";
	else
		for ( int j= 0 ; j< statement.size() ; j++ )
			cout<< statement[j] << endl;

}
void card::write_to_file_card(ostream& output){
	if(description == "" ) description = '*';
	if(date == "" ) date = '*';
	output << "first_of_card" << endl;
	output << name << ' ' << card_number << ' ' << description << ' ' << date << ' ' << endl;
	for ( int i =0; i< worker.size() ; i++ )
		output << worker[i].name << ' ' << worker[i].username << ' ' << worker[i].password << endl;
	output << '&' << endl;
	for ( int i =0; i< statement.size() ; i++ )
		output<< statement[i] << endl;
	output << '%' << endl;
}

bool card:: read_from_file_card (istream& input ){
	string nam,descript , dt;
	int card_n;
	input >>nam;
	if (nam == "without_cards")
		return false;
	if(nam != "first_of_card"){
		name = nam;
		input >> card_number >> descript >> dt ;
	}
	else
	input >> name >> card_number >> descript >> dt ;
	if ( descript == "*" ) descript = "";
	if (dt == "*" ) dt = "";
	description = descript;
	date = dt;
	input >> nam;
	while(nam != "&" ){
		string user_n , pass;
		input >> user_n >> pass;
		vector<string> information;
		information.push_back(nam);
		information.push_back(user_n);
		information.push_back(pass);
		user temp_worker;
		temp_worker.signup(information);
		worker.push_back(temp_worker);
		input >> nam;
	}
	input >>nam;
	while(nam != "%" ){
		string sentence;
		getline(input,sentence);
		sentence = nam + sentence;
		statement.push_back(nam);
		input >> nam;
	}
	return true;
}

void card::create_card( string c_name , int numb ) {
	card_number = numb;
	name = c_name;
}
////////////////////////////////////////////////////// USER

void user :: signup ( vector<string> input ){
	name = input[0];
	username = input[1];
	password = input[2];
	signin = false;
}


////////////////////////////////////////////////////// list
void list::write_to_list(ostream& output){
	output << "first_of_list" << endl;
	output << name << ' ' << list_number << endl;
	if ( cards.size() == 0 )
		output << "without_cards" << endl;
	for(int i=0; i<cards.size() ;i++ )
		cards[i]->write_to_file_card(output);
	output<<"end_of_list" << endl;
}
bool list::read_from_list(istream& input){
	string nam;
	input >>nam;
	if ( nam == "no_list" )
		return false;
	if (nam != "first_of_list" ){
		name = nam;
		input >> list_number;
	}
	else
	input >> name >> list_number;
	while (nam != "end_of_list" ){
		card* temp_card = new card();
		if(temp_card->read_from_file_card(input))
			cards.push_back(temp_card);
		input >> nam;
	}
	return true;
}

list:: ~list(){
	for(int i=0; i< cards.size() ; i++ )
		delete cards[i];
}
bool list::show_card ( int card_numb ) {
	for ( int i = 0; i< cards.size() ; i++ )
		if ( card_numb == cards[i]->card_number ){
			cards[i]->show_card();
			return true;
		}
	return false;
}

void list::add_card( string c_name, int _number ) {
	card* temp;
	temp->create_card(c_name,_number);
	cards.push_back(temp);
}

void list::show_list(){
	cout << "The list name is: " << name << " , and the its number is : " << list_number << endl;
}

void list::create_list(string nam, int numb){
	name=nam;
	list_number = numb;
}