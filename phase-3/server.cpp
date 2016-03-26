#include "header.h"

#define _client 2


int main(int argc , char* argv[]){
  
  int sockfd, portno;
  if(argc<2)
    perror("No port provided!");
  portno=atoi(argv[1]);

  struct sockaddr_in serv_addr;

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(sockfd<0)
    perror("opennig socket");

  if(bind(sockfd, (struct sockaddr* ) &serv_addr, sizeof(serv_addr) )<0)
    perror("On binding");
/////////////////////////setting the program!
	trello program;
	Thread t[_client];
	program.set_sockfd(sockfd);
	program.read_from_file();

	for( int i=0 ; i<_client ; i++)
		t[i].start( program );
	 
	for( int i=0 ; i<_client ; i++)
		t[i].join( );		
	
	program.write_to_file();
    cout<<"Program saved!"<<endl;
  close(sockfd);
	return 0;
}

