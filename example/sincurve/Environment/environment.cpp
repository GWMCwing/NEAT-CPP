#include "./environment.h"
#include <cmath>
#include "../util/randomFun.h"
// modify or add code after //TODO_ADD_CODE comment
namespace neatCpp {
    std::vector<long double> generateSinVector(int size, std::vector<long double>& inputDouble) {
        std::vector<long double> r = std::vector<long double>();
        r.reserve(size);
        // input output
        for (int i = 0; i < size; ++i) {
            long double input = randNum(0, 2 * M_PI);
            inputDouble.push_back(input);
            r.push_back(sin(input));
        }
        return r;
    }
    void Environment::updateOnce() {
        if (envExiting || envExited) return;
        const std::lock_guard<std::mutex> lock(envUpdateMutex);
        //TODO_ADD_CODE
        int trainSize = 50;
        std::vector<long double> inputDouble = std::vector<long double>();
        inputDouble.reserve(trainSize);
        std::vector<long double> outputDouble = generateSinVector(trainSize, inputDouble);
        std::vector<Player*> populationPlayers = population->getPopulationPlayers();
        int populationSize = population->getPopulationSize();
        for (int i = 0; i < trainSize; ++i) {
            std::vector<long double> inputDoubleOne = std::vector<long double>();
            std::vector<long double> outputDoubleOne = std::vector<long double>();
            inputDoubleOne.push_back(inputDouble[i]);
            outputDoubleOne.push_back(outputDouble[i]);
            for (int j = 0;j < populationSize; ++j) {
                populationPlayers[j]->train(inputDoubleOne, outputDoubleOne);
            }
        }
        for (int j = 0;j < populationSize; ++j) {
            populationPlayers[j]->setDead(true);
        }
        //
        population->naturalSelection();
    }
    Environment::Environment(int populationSize, int inputSize, int outputSize, OutputBuffer* _outputBuffer, std::mutex& _envUpdateMutex)
        : envUpdateMutex(_envUpdateMutex), outputBuffer(_outputBuffer) {
        const std::lock_guard<std::mutex> lock(envUpdateMutex);
        population = new Population(populationSize, inputSize, outputSize, outputBuffer);
        envExiting = false;
        envExited = false;
    }
    Environment::~Environment() {
        const std::lock_guard<std::mutex> lock(envUpdateMutex);
        delete population;
        //! do not delete outputBuffer, it is handled by CLI
    }

    void Environment::exitEnv() {
        envExiting = true;
        // await for update thread or thread that require synchronous access of Environemnt object
        const std::lock_guard<std::mutex> lock(envUpdateMutex);
        envExited = true;
    }
    void Environment::exportPopulation(std::fstream& exportStream) const {
        population->exportPopulation(exportStream);
    }
    void Environment::exportBestPlayer(std::fstream& exportStream) const {
        population->exportBestPlayer(exportStream);
    }
    //
    long double Environment::getBestScore() const { return population->getBestPlayerScore(); }
    // int Environment::getBestScoreGeneration() const { return population->getBestPlayerGeneration(); }
    long double Environment::getAverageScore() const { return population->getAverageScore(); }
    int Environment::getGeneration() const { return population->getGeneration(); }
    bool Environment::isEnvExiting() const { return envExiting; }
    bool Environment::isEnvExited() const { return envExited; }
}