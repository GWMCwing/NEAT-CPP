#ifndef NEATCPP_ENVIRONMENT_HEADER_H
#define NEATCPP_ENVIRONMENT_HEADER_H

#include <queue>
#include <mutex>
#include <atomic>

#include "./population.h"
#include "./outputBuffer.h"
namespace neatCpp {
    class Environment {
    public:
        // user defined functions
        // Environment required functions
        Environment(int populationSize, int inputSize, int outputSize, OutputBuffer* _outputBuffer, std::mutex& _envUpdateMutex);
        ~Environment();
        void updateOnce();
        // cli required functions
        long double getBestScore() const;
        // int getBestScoreGeneration() const;
        long double getAverageScore() const;
        int getGeneration() const;
        bool isEnvExiting() const;
        bool isEnvExited() const;
        void exitEnv();
        //TODO
        void exportPopulation(std::fstream& exportStream) const;
        void exportBestPlayer(std::fstream& exportStream) const;
    private:
        // user defined members
        // user defined functions
        // Environment required functions
        // Environment required members
        Population* population;
        OutputBuffer* const outputBuffer;
        // cli required members
        mutable std::mutex& envUpdateMutex;
        std::atomic<bool> envExiting;
        std::atomic<bool> envExited;
    };
}
#endif