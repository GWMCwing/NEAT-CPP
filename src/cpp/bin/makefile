ObjFiles:= activationFunction.o randomFun.o connection.o genome.o node.o player.o population.o 

nn.exe: $(ObjFiles)
	g++ -o nn.exe $(ObjFiles) 

connection.o: ../connection.cpp
	g++ -c ../connection.cpp -o connection.o

genome.o: ../genome.cpp
	g++ -c ../genome.cpp -o genome.o

node.o: ../node.cpp
	g++ -c ../node.cpp -o node.o

player.o : ../player.cpp
	g++ -c ../player.cpp -o player.o

population.o : ../population.cpp
	g++ -c ../population.cpp -o population.o

randomFun.o : ../randomFun.cpp
	g++ -c ../randomFun.cpp -o randomFun.o

activationFunction.o: ../activationFunction.cpp
	g++ -c ../activationFunction.cpp -o activationFunction.o