#include "header.h"


bool perror(string error){
  cout<<"ERROR: "<<error<<endl;
  exit(0);
  return true;
}

int main(int argc , char* argv[]){

  int ex=false;

  //declarations
  char buffer[256];
  int sockfd, client_fd, portno, n;
  if(argc<2)
    perror("No port provided!");
  portno=atoi(argv[1]);

  bzero(buffer, sizeof buffer);
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
	program.read_from_file();

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
		
		string res = program.handle_program_event( string(buffer) );
		char* result=strdup(res.c_str());
		n=write(client_fd, result, 255);
		if ( res == "You exited!" ){
			ex = true;
			break;
		}
		bzero(result, sizeof(result));
		if(n<0)
			perror("On writing");
    
    }
    	program.write_to_file();
    	cout<<"Program saved!"<<endl;
    	close(client_fd);
  }
	
  close(sockfd);
	return 0;
}

