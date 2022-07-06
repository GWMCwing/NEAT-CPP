#include "./CLI/cli.h"
#include "./NeuralNet/population.h"
int main() {
    int populationSize = 50;
    int inputSize = 1;
    int outputSize = 1;
    std::ofstream logFile("./logFile.txt");
    neatCpp::CLI cli(populationSize, inputSize, outputSize, 50, logFile);
    logFile.close();
    // std::mutex outputBufferMutex;
    // neatCpp::OutputBuffer* outputBuffer = new neatCpp::OutputBuffer(outputBufferMutex);
    // neatCpp::Population population(populationSize, inputSize, outputSize, outputBuffer);
    // delete outputBuffer;
}