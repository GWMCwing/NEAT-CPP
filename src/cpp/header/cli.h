#ifndef NEATCPP_CLI_HEADER_H
#define NEATCPP_CLI_HEADER_H

#include <mutex>
#include <atomic>
#include <fstream>
#include <thread>

#include "./environment.h"
#include "./outputBuffer.h"
namespace neatCpp {
    class CLI {
    public:
        CLI(int populationSize, int inputSize, int outputSize, int _expectedGeneration, std::ofstream& logFileStream);
        void updateDisplay();
        bool updateOnce();
        void flushToLogFile();
        bool startContinuousUpdate();
        void handleUserInput();
        void exitCli();
        void stopEnv();
        void exportPopulation(std::fstream& exportStream) const;
        void exportBestPlayer(std::fstream& exportStream) const;
    private:
        // define the position of the display
        // starts with 1,1 as the top left
        int infoBlockLine;
        int infoBlockHeight;
        int logStartLine;
        int logHeight;
        int inputLine;
        std::atomic<int> flushedLogId;
        //
        std::ofstream& logFile;
        OutputBuffer* outputBuffer;
        Environment* env;
        int expectedGeneration;
        long double previousBestScore;
        long double previousAverageScore;
        mutable std::mutex envUpdateMutex;
        mutable std::mutex bufferUpdateMutex;
        mutable std::mutex displayUpdateMutex;

        //
        void updateLogHeight(int value);
        void continuousUpdateEnvironment();
        void awaitEnvExit();
        void awaitBufferClear();
        void getDurationStr(std::string& durationStr) const;
        void getBestScoreStr(std::string& bestScoreStr, std::string& bestScoreDeltaStr) const;
        void getAverageScoreStr(std::string& averageScoreStr, std::string& averageScoreDeltaStr) const;
        std::string formatString(Log log) const;
        std::string formatString_colored(Log log) const;
        std::thread envThread;
        std::atomic<bool> envThreadStarted;
        std::time_t startTime;

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
