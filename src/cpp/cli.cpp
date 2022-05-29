// TODO user Input
// TODO pipe population details to logFile
// TODO async buffer for piping to logFile
#include <thread>
#include <chrono>
#include <iostream>


#include "./header/cli.h"

using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

int main() {
    std::ofstream logFile = initiateFile("./log.txt");
    //
    // create thread for env
    neatCpp::Environment* env = new neatCpp::Environment(100, 2, 3);
    std::thread envThread = std::thread(neatCpp::updateEnvironment, env);
    // await user input
    // pipe input to both terminal and logFile
    std::string userInput = "";
    volatile bool exitCli = false;
    while (!exitCli) {
        cinString(userInput, logFile);
        handleUserInput(userInput, exitCli);
    }
    //
    coutString("Awaiting Environment to Exit...", logFile);
    envThread.join();
    logFile.close();
    coutString("Environment Exited", logFile);
    return 0;
}
//
//
std::ofstream initiateFile(std::string path) {
    std::ofstream logFile(path, std::ios::app);
    const std::time_t startTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string str = "\n\nCreateTime: "; str += (std::ctime(&startTime));
    coutString(str, logFile);
    return logFile;
}
//! only call this function in main thread, non-buffered fstream
void coutString(const std::string& str, std::ofstream& logFile) {
    std::cout << str << std::endl;
    logFile << str << std::endl;
}
//! only call this function in main thread, non-buffered fstream
void cinString(std::string& str, std::ofstream& logFile) {
    std::cin >> str;
    logFile << "> " << str << std::endl;
}
void handleUserInput(const std::string& str, volatile bool& exitCli) {

}
//
//
namespace neatCpp {
    void updateEnvironment(Environment* const& env) {
        while (env->getEnvironmentUpdateState() >= 0) {
            if (env->getEnvironmentUpdateState() > 0) {
                sleep_for(25ms);
                continue;
            }
            env->updateEnvironment();
            sleep_for(50ms);
        }
        // wait for Env to save all the data
        std::thread awaitEnvExitThread = std::thread(awaitEnvExit, env);
        awaitEnvExitThread.join();
    }
    void awaitEnvExit(Environment*& env) {
        env->AddToEvent(CommandId::EXIT);
        while (env->getEnvironmentUpdateState() != EnvironmentState::EXITED) {
            sleep_for(50ms);
        }
        delete env;
        env = nullptr;
    }
}