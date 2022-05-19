nn.exe: randomFun.o connection.o genome.o node.o player.o population.o 
	g++ -o nn.exe ./obj/randomFun.o ./obj/connection.o ./obj/genome.o ./obj/node.o ./obj/player.o ./obj/population.o

connection.o: connection.cpp
	g++ -c connection.cpp -o ./obj/connection.o

genome.o: genome.cpp
	g++ -c genome.cpp -o ./obj/genome.o

node.o: node.cpp
	g++ -c node.cpp -o ./obj/node.o

player.o : player.cpp
	g++ -c player.cpp -o ./obj/player.o

population.o : population.cpp
	g++ -c population.cpp -o ./obj/population.o

randomFun.o : randomFun.cpp
	g++ -c randomFun.cpp -o ./obj/randomFun.o
