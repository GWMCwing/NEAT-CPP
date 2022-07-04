#ifndef NEATCPP_ENVIRONMENT_HEADER_H
#define NEATCPP_ENVIRONMENT_HEADER_H

#include <queue>
#include <mutex>
#include <atomic>

#include "../NeuralNet/population.h"
#include "../CLI/outputBuffer.h"
namespace neatCpp {
    class Environment {
    public:
        // user defined functions
        // Environment required functions
        /**
         * @brief Construct a new Environment object
         *
         * @param populationSize population size of the training environment
         * @param inputSize input size of neural network
         * @param outputSize output size of the neural network
         * @param _outputBuffer pointer to a OutputBuffer object for logging handling across threads
         * @param _envUpdateMutex reference of a mutex created from parent object, use to perform lock guard of async functions
         */
        Environment(int populationSize, int inputSize, int outputSize, OutputBuffer* _outputBuffer, std::mutex& _envUpdateMutex);
        /**
         * @brief Destroy the Environment object, uses envUpdateMutex for lock guard, deallocate populations
         */
        ~Environment();
        /**
         * @brief update the environment once
         *
         */
        void updateOnce();
        // cli required functions
        /**
         * @brief Get the Best Score of the best player among all generations
         *
         * @return long double best score
         */
        long double getBestScore() const;
        /**
         * @brief Get the Average Score of the last generation
         *
         * @return long double average score
         */
        long double getAverageScore() const;
        /**
         * @brief Get the number Generation currently is
         *
         * @return int number of generate
         */
        int getGeneration() const;
        /**
         * @brief check if the environment is instructed or processed to exits, or stop
         *
         * @return true the environment is stopping, false otherwise
         */
        bool isEnvExiting() const;
        /**
         * @brief check if the environment has exited or stopped
         *
         * @return true the environment stopped, false otherwise
         */
        bool isEnvExited() const;
        /**
         * @brief instruct the environment to stop and save all data
         *
         */
        void exitEnv();
        //TODO
        /**
         * @brief export the population to the exportStream with format defined in Population.cpp
         *
         * @param exportStream reference of a fstream to be exported to
         */
        void exportPopulation(std::fstream& exportStream) const;
        /**
         * @brief export the best player to the exportStream with format defined in Population.cpp
         *
         * @param exportStream reference of a fstream to be exported to
         */
        void exportBestPlayer(std::fstream& exportStream) const;
    private:
        // user defined members
        // user defined functions
        // Environment required functions
        // Environment required members
        /** pointer to the population object */
        Population* population;
        /** pointer to the outputBuffer object */
        OutputBuffer* const outputBuffer;
        // cli required members
        /** mutex for performing lock guard for async functions */
        mutable std::mutex& envUpdateMutex;
        /** boolean for signaling if the environemnt is instructed to stop or exit*/
        std::atomic<bool> envExiting;
        /** boolean for signaling if the environment is in a exited state */
        std::atomic<bool> envExited;
    };
}
#endif