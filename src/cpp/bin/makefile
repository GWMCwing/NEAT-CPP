ObjFiles:=  outputBuffer.o connection.o genome.o node.o population.o randomFun.o activationFunction.o cli.o
rootPath:= $(realpath .)

generate: $(ObjFiles)
	@echo 'Generating all base object files, Envrionment.o requires seperate compilation from modified Enviroment.cpp'

connection.o: ../NeuralNet/connection.cpp
	cd $(rootPath)
	g++ -c ../NeuralNet/connection.cpp -o ../bin/connection.o

genome.o: ../NeuralNet/genome.cpp
	cd $(rootPath)
	g++ -c ../NeuralNet/genome.cpp -o ../bin/genome.o

node.o: ../NeuralNet/node.cpp
	cd $(rootPath)
	g++ -c ../NeuralNet/node.cpp -o ../bin/node.o

# player.o : ../NeuralNet/player.cpp
# 	cd $(rootPath)
# 	g++ -c ../NeuralNet/player.cpp -o ../bin/player.o

population.o : ../NeuralNet/population.cpp
	cd $(rootPath)
	g++ -c ../NeuralNet/population.cpp -o ../bin/population.o

randomFun.o : ../util/randomFun.cpp
	cd $(rootPath)
	g++ -c ../util/randomFun.cpp -o ../bin/randomFun.o

activationFunction.o: ../util/activationFunction.cpp
	cd $(rootPath)
	g++ -c ../util/activationFunction.cpp -o ../bin/activationFunction.o

cli.o: ../CLI/cli.cpp
	cd $(rootPath)
	g++ -c ../CLI/cli.cpp -o ../bin/cli.o

outputBuffer.o: ../CLI/outputBuffer.cpp
	cd $(rootPath)
	g++ -c ../CLI/outputBuffer.cpp -o ../bin/outputBuffer.o