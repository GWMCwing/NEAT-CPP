generate:
	g++ -pthread -fsanitize=address,leak,undefined -o sinExample ./util/randomFun.cpp ./util/activationFunction.cpp ./CLI/cli.cpp ./CLI/outputBuffer.cpp ./NeuralNet/connection.cpp ./NeuralNet/genome.cpp ./NeuralNet/node.cpp \
	./NeuralNet/player.cpp ./Environment/environment.cpp ./NeuralNet/population.cpp  main.cpp