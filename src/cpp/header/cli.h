#ifndef NEATCPP_CLI_HEADER_H
#define NEATCPP_CLI_HEADER_H
#include <string>
#include <fstream>

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
}
#endif