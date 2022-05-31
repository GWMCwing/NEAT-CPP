#ifndef NEATCPP_CLI_HEADER_H
#define NEATCPP_CLI_HEADER_H
#include <string>
#include <fstream>
#include <atomic>

#include "./environment.h"
void coutString(const std::string& str, std::ofstream& logFile);
void cinString(std::string& str, std::ofstream& logFile);
std::ofstream initiateFile(std::string path);
void handleUserInput(const std::string& str, volatile bool& exitCli);
namespace neatCpp {
    void updateEnvironment(Environment* const& env);
    void awaitEnvExit(Environment*& env);
    bool exportPopulation(std::string path);
    bool exportBestPlayer(std::string path);
    //TODO Stable buffer implementation
    class OutputBuffer {
        //! non-stable buffer implementation
    public:
        OutputBuffer(std::ofstream& _logFile);
        OutputBuffer& operator<<(std::string str);
    private:
        // determine if there is any thread blocking operation, 0 means no thread blocking, 1 means thread blocking
        std::atomic<bool> blocked;
        std::atomic<int> waitingThread;
        std::ofstream& logFile;
        // called by << operator and HAVE to be detached from thread to prevent thread blocking
        void outputStr(std::string str);
    };
}
#endif