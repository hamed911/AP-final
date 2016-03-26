#include<iostream>
#include<vector>
#include<string>
#include<cstdlib>
#include<fstream>
#include<cstring>
using namespace std;

vector<string> input_for_signup ();
vector<string> get_two_input();
vector<string> get_two_long_input();
string get_one_input();
void check_no_more_input();
vector<string> get_filter_input();
string check_date_input(string date);


class user {
//private:
public:
	string name;
	string username;
	string password;
	bool signin;

	user(){
		signin = false;
	}
	void signup( vector<string> );
};

class man {
private:
	vector<user> people;
	user* current_user;
	friend class board;
	friend class trello;
public:
	man () { current_user = NULL; }
	void signup();
	void signin();
	void signout();
	bool handle_event( string order );
	bool check_existence ( string id );
};

class card {
private:
	int card_number;
	string name;
	string description;
	string date;
	vector<user> worker;
	vector<string> statement;
public:
	friend class list;
	friend class board;
	void write_to_file_card(ostream& output);
	bool read_from_file_card(istream& input);
	card(){ card_number = 0; }
	void create_card( string c_nam , int numb );
	void show_card();
	void rename_card( string new_name );
	void edit_card_description ( string descript );
	void edit_due_date ( string d );
	void assign_user ( user new_worker );
	bool remove_user ( string username );
	void comment ( string cm  );
};

class list {
private:
	int list_number;
	string name;
	vector<card*> cards;
public:
	friend class board;
	list(){list_number = 0;}
	~list();
	void write_to_list(ostream & output);
	bool read_from_list(istream & input);
	bool show_card( int );
	void add_card ( string c_name, int _number);
	void show_list();
	void create_list(string , int); 

};

class board {
private:
	int card_counter;
	int list_counter;
	int board_number;
	string board_name;
	string maker;
	vector<list> _list;
	man members;
public:
	void read_from_file_board(istream& input);
	void write_to_file_board(ostream& output);
	board(){ card_counter =1; list_counter = 1;}
	friend class trello;
	void show_cards_of_list();
	vector<card*> find_in_commen_card(vector<string> nam);
	list* find_list_of_card(int card_numb );
	card* find_card_pointer(int card_num );
	list* find_list ( int list_num);
	bool remove_card(int);
	void remove_special_card();
	void check_existence_security();
	bool handle_board_event(string order);
	void new_board(string admin, string b_name , int board_numb,int c_counter =1 ,int l_counter =1);
	bool show_board();
	void show_list();
	void add_list();
	void remove_list ();
	void add_new_card();
	bool add_card( string card_name , int list_numb );
	void move_card ();
	void rename_card();
	void edit_card_description();
	void edit_card_due_date ();
	void assign_user ();
	void remove_user ();
	void comment ();
	void filter();
};

class trello {
private:
	man all_user;
	int board_counter;
	vector<board> _board;
	board* current_board;
public:
	trello() { current_board = NULL; board_counter = 1; } 
	void read_from_file();
	void write_to_file();
	void update_board_current_user();
	bool handle_program_event(string);
	void show_board();
	void enter_board();
	void create_board();
	void remove_board();
	void add_user();
	void remove_user_from_board();
};
