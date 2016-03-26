#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <iostream>
using namespace std;

void error(string msg)
{
  cout<<"ERROR:: "<<msg<<endl;
  exit(0);
}

int main()
{
  //declarations
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  string line;
  string def;
  string IP;
  int port;
  bool connected=false;
  int ex=false;


  while(!ex){
    getline(cin, line);
    stringstream ss;
    ss<<line;
    ss>>def;
	
    //checking connect & exit and disconnect commands
    if(!connected && def=="connect"){
      ss>>IP;
      ss>>port;
      portno=port;
      connected=true;
    }
    else if(!connected && def=="disconnect")
      cout<<"You are not connected"<<endl;
	else 
      cout<<"ERROR!"<<endl;
   if(def=="exit")
	return 0;
    //end of checking

    char buffer[256];


    //create socket, connect, 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
      error("opening socket");
    server = gethostbyname(IP.c_str());

    if (server == NULL) {
      error("no such host");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
      cout<<"Error on connecting"<<endl;
      connected=false;
    }
    else
      cout<<"connected"<<endl;

    
    // after connecting process commands
    while(connected){
     
      string cmd;
      getline(cin, cmd);

      char* b;
      b=strdup(cmd.c_str());

      if(b=="exit"){
	connected = false;
	}
      n = write(sockfd,b,strlen(b));
      if(cmd=="disconnect"){
	connected=false;
      }
      if (n < 0)  error(" writing to socket");
      //string a= string(b);
      bzero(buffer, sizeof(buffer));


      while(true){
	n = read(sockfd,buffer,255);
	if (n < 0)  error("reading from socket"); 
	if(n>1) break;
      }
	cout<<buffer;
      if(string(buffer)=="You exited!"){
	ex=true;
	break;
      }
   
      
    }
   
  	close(sockfd); 
  }
  
  return 0;
}
