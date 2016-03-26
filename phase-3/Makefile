server: server.o board.o function.o all_user.o program.o
	g++ server.o board.o function.o all_user.o program.o -o server -lPocoFoundation -lPocoXML
server.o: server.cpp
	g++ -c server.cpp -lPocoFoundation -lPocoXML
program.o: program.cpp
	g++ -c program.cpp -lPocoFoundation -lPocoXML
function.o: function.cpp
	g++ -c function.cpp -lPocoFoundation -lPocoXML
board.o: board.cpp
	g++ -c board.cpp -lPocoFoundation -lPocoXML
all_user.o: all_user.cpp
	g++ -c all_user.cpp -lPocoFoundation -lPocoXML
