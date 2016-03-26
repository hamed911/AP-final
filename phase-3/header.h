#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <Poco/Thread.h>

using namespace Poco;
using namespace std;

vector<string> input_for_signup (string command);
vector<string> get_two_input(string command);
vector<string> get_two_long_input(string command);
string get_one_input(string command);
void check_no_more_input(string command);
vector<string> get_filter_input(string command);
string check_date_input(string date);
string erase_command( string command );
bool perror(string error);

class user {
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
	void signup(string command,ostringstream& out);
	void signin(string command,ostringstream& out);
	void signout(string command,ostringstream& out);
	bool handle_event( string command ,ostringstream& out);
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
	void show_card(ostringstream& out);
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
	bool show_card( int , ostringstream& );
	void add_card ( string c_name, int _number);
	void show_list(ostringstream& out);
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
	void show_cards_of_list(string command,ostringstream& out);
	vector<card*> find_in_commen_card(vector<string> nam);
	list* find_list_of_card(int card_numb );
	card* find_card_pointer(int card_num );
	list* find_list ( int list_num);
	bool remove_card(int);
	void remove_special_card(string command,ostringstream& out);
	void check_existence_security();
	bool handle_board_event(string order,ostringstream&);
	void new_board(string admin, string b_name , int board_numb,int c_counter =1 ,int l_counter =1);
	bool show_board(ostringstream& out);
	void show_list(string command,ostringstream& out);
	void add_list(string command,ostringstream& out);
	void remove_list (string command,ostringstream& out);
	void add_new_card(string command,ostringstream& out);
	bool add_card( string card_name , int list_numb );
	void move_card (string command,ostringstream& out);
	void rename_card(string command,ostringstream& out);
	void edit_card_description(string command,ostringstream& out);
	void edit_card_due_date (string command,ostringstream& out);
	void assign_user (string command,ostringstream& out);
	void remove_user (string command,ostringstream& out);
	void comment (string command,ostringstream& out);
	void filter(string command,ostringstream& out);
};

class trello :public Runnable {
private:
	
	int sockfd;
	Mutex mutex;
	ostringstream out;
	man all_user;
	int board_counter;
	vector<board> _board;
	board* current_board;
	
public:
	trello() { current_board = NULL; board_counter = 1; }
	void set_sockfd( int sock ) { sockfd = sock; }
	void read_from_file();
	void write_to_file();
	void update_board_current_user();
	string handle_program_event(string command);
	void show_board(string command,ostringstream& out);
	void enter_board(string command);
	void create_board(string command);
	void remove_board( string command);
	void add_user(string command);
	void remove_user_from_board(string command);




/////////////////////////////

	virtual void run(){
	
		int ex=false;
		int client_fd,n;
		char buffer[256];
		bzero(buffer, sizeof buffer);
		while(!ex){
		if(listen(sockfd, 5)==-1)
		  perror("on listening");
		
		cout<<"Listening..."<<endl; 
		struct sockaddr_in client_addr;
		socklen_t client_addr_len= sizeof(client_addr);
		
		client_fd = accept(sockfd, (struct sockaddr*)& client_addr, &client_addr_len);


		if( client_fd<0) 
		  perror("on accept");

		cout<<"accepted"<<endl;

		while(true){
		  bzero(buffer, sizeof(buffer));
		  n=read(client_fd, buffer, sizeof(buffer) );
		  if(n<0)
			perror("On reading");
		  cout<<buffer<<endl;

		  if(string(buffer)=="disconnect")
			break;
		
			string res = handle_program_event( string(buffer) );


			char* result=strdup(res.c_str());
			n=write(client_fd, result, 255);
			if ( res == "You exited!" ){
				break;
			}
			bzero(result, sizeof(result));
			if(n<0)
				perror("On writing");
		
		}
			
			close(client_fd);
	  }


	}//run
};
