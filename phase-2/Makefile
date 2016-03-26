server: server.o board.o function.o all_user.o program.o
	g++ server.o board.o function.o all_user.o program.o -o server
server.o: server.cpp
	g++ -c server.cpp
program.o: program.cpp
	g++ -c program.cpp
function.o: function.cpp
	g++ -c function.cpp
board.o: board.cpp
	g++ -c board.cpp
all_user.o: all_user.cpp
	g++ -c all_user.cpp
