#include "./environment.h"
// modify or add code after //TODO_ADD_CODE comment
namespace neatCpp {
    void Environment::updateOnce() {
        if (envExiting || envExited) return;
        const std::lock_guard<std::mutex> lock(envUpdateMutex);
        //TODO_ADD_CODE
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
    //
    long double Environment::getBestScore() const { return population->getBestPlayerScore(); }
    // int Environment::getBestScoreGeneration() const { return population->getBestPlayerGeneration(); }
    long double Environment::getAverageScore() const { return population->getAverageScore(); }
    int Environment::getGeneration() const { return population->getGeneration(); }
    bool Environment::isEnvExiting() const { return envExiting; }
    bool Environment::isEnvExited() const { return envExited; }
}