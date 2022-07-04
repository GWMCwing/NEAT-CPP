#ifndef NEATCPP_CLI_HEADER_H
#define NEATCPP_CLI_HEADER_H

#include <mutex>
#include <atomic>
#include <fstream>
#include <thread>

#include "../Environment/environment.h"
#include "./outputBuffer.h"
namespace neatCpp {
    class CLI {
    public:
        /**
         * @brief Construct a new CLI object
         *
         * @param populationSize population size for Environment population
         * @param inputSize inputSize of a neural network
         * @param outputSize outputSize of a neural network
         * @param _expectedGeneration auto export population data when expectedGeneration is reached
         * @param logFileStream ofstream for the logFile of the cli and environment
         */
        CLI(int populationSize, int inputSize, int outputSize, int _expectedGeneration, std::ofstream& logFileStream);
        /** @brief flush and update the cli display*/
        void updateDisplay();
        /** @brief update the environment once, directly invoke Environemnt::updateOnce*/
        bool updateOnce();
        /** @brief flush all pending log in OutputBuffer to logFile*/
        void flushToLogFile();
        /** @brief create a detached thread for continuously update the environment with interval of 5ms
         * Environemnt::continuousUpdateEnvironment is called in thread, can only be called once
        */
        bool startContinuousUpdate();
        /** @brief input handler for possible input from cli*/
        void handleUserInput();
        /** @brief stop cli, flush all output logs, exit environemnt and release all allocated memory */
        void exitCli();
        //TODO ? will Environment release all memory after this?
        /** @brief stop Environemnt from updating*/
        void stopEnv();
        /**
         * @brief flush population data to exportStream
         *
         * @param exportStream destination fstream for population data
         */
        void exportPopulation(std::fstream& exportStream) const;
        /**
         * @brief flush historical best player data to exportStream
         *
         * @param exportStream destination fstream for historical best player data
         */
        void exportBestPlayer(std::fstream& exportStream) const;
    private:
        // define the position of the display
        // starts with 1,1 as the top left
        /** @brief number of line the info block starts*/
        int infoBlockLine;
        /** @brief number of lines the info block occupy*/
        int infoBlockHeight;
        /** @brief number of line the log block starts*/
        int logStartLine;
        /** @brief number of lines the log block occupy*/
        int logHeight;
        /** @brief number of line the input line*/
        int inputLine;
        /** @brief log id for flushing, atomic for async access of log flushing and appending*/
        std::atomic<int> flushedLogId;
        //
        /** @brief reference of ofstream of destination of logFile*/
        std::ofstream& logFile;
        /** @brief Buffer for handling all flushing to display and logFile*/
        OutputBuffer* outputBuffer;
        /** @brief Object of the training Environemnt*/
        Environment* env;
        /** @brief Generation that triggers a auto export of population*/
        int expectedGeneration;
        /** @brief save of previous generation best score for updating best player*/
        long double previousBestScore;
        /** @brief save of previous average score to calculate changes between generation*/
        long double previousAverageScore;
        /** @brief Mutex for locking Environemnt state from exporting and updating*/
        mutable std::mutex envUpdateMutex;
        /** @brief Mutex for locking output buffer state from flushing and appending*/
        mutable std::mutex bufferUpdateMutex;
        /** @brief Mutex for updating display and flushing log to display*/
        mutable std::mutex displayUpdateMutex;

        //
        /** @brief update lines of log block occupy in cli
         * @param value lines to be set
        */
        void updateLogHeight(int value);
        /** @brief update Environment continuously with interval of 5ms*/
        void continuousUpdateEnvironment();
        /** @brief thread blocking process for waiting Environemnt to be completely safe to exit*/
        void awaitEnvExit();
        /** @brief thread blocking process for waiting output buffer to be completely safe to exit*/
        void awaitBufferClear();
        /** @brief generate formatted duration string
         * @param durationStr reference of a string to be returned from the function
        */
        void getDurationStr(std::string& durationStr) const;
        /**
         * @brief generate best score string and best score delta string for cli
         *
         * @param bestScoreStr reference of a string to be returned from the function for best score
         * @param bestScoreDeltaStr reference of a string to be returned from the function for best score delta
         */
        void getBestScoreStr(std::string& bestScoreStr, std::string& bestScoreDeltaStr) const;
        /**
         * @brief generate average score string and average score delta string
         *
         * @param averageScoreStr reference of a string to be returned from the function for average score
         * @param averageScoreDeltaStr reference of a string to be returned from the function for average score delta
         */
        void getAverageScoreStr(std::string& averageScoreStr, std::string& averageScoreDeltaStr) const;
        /**
         * @brief format a log object into a string for output and return it, without color
         *
         * @param log a Log object containing infomation to be output
         * @return std::string formatted string
         */
        std::string formatString(Log log) const;
        /**
         * @brief format a log object into a string for output and return it, with color
         *
         * @param log a Log object containing infomation to be output
         * @return std::string formatted string
         */
        std::string formatString_colored(Log log) const;
        /** @brief thread of the environemnt update*/
        std::thread envThread;
        /** @brief boolean representing the is Environemnt thread is running*/
        std::atomic<bool> envThreadStarted;
        /** @brief storing the time of starting time*/
        std::time_t startTime;
        /** @brief max number of character to be displayed for each number displayed*/
        const int maxNumberChar = 8;
    };
}
#endif
// display format
/*
1   Duration: 00:00:00 Generation: 1
2   Best Score: (), Delta: (), From Generation: 1
3   Avg Score: (), Delta: ()
4   Environment State:
5   ------------------ (log height is determined by a member in CLI)
6   (log)
7   (log)
8   (log)
9   (log)
10  (log)
11  (log)
12  (log)
13  (log)
14  > (input)
*/
