#include"header.h"

int main () {
	string order;
	
	trello program;
	program.read_from_file();
	
	cin >> order;
	while ( order != "exit" ) {
		try {
	
			if( !program.handle_program_event(order) )
				throw "You enter wrong command,This command doesn't exist!Please try again!";

		} catch( char const* error ){
			cout<< error << endl;
		}//catch

		cin >> order;
	}
	program.write_to_file();
	return 0;
}// main