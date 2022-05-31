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
    time_t timeStart = time(NULL);
    while (!exitCli) {
        updateDisplay(env, logFile, timeStart);
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
std::ofstream initiateFile(const std::string path) {
    std::ofstream logFile(path, std::ios::app);
    const std::time_t startTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string str = "\n\nCreateTime: "; str += (std::ctime(&startTime));
    coutString(str, logFile);
    return logFile;
}
//! only call this function in main thread, non-buffered fstream
void updateDisplay(const neatCpp::Environment* const& env, std::ofstream& logFile, const time_t& timeStart) {
    const time_t deltaTime = time(NULL) - timeStart;
    const int hour = deltaTime / 3600;
    const int minute = (deltaTime / 60) % 60;
    const int sec = deltaTime % 60;
    static long double previousScore = -1;
    // display Generation best player score, average score, delta score from previous

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
    if (str == "export") {
        return;
    }
    if (str == "stop") {
        //TODO ask for export
        return;
    }
    if (str == "pause") {
        return;
    }
    if (str == "resume") {
        return;
    }
    if (str == "help") {
        return;
    }
}
//
//
namespace neatCpp {
    //? this should call Environment member only, non-atomic comparison
    void updateEnvironment(Environment* const& env) {
        //! possible race condition
        while (env->getEnvironmentUpdateState() >= 0) { // not exit nor stopping
            if (env->getEnvironmentUpdateState() > 0) { // is thread blocking
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
        env->addToEvent(CommandId::EXIT);
        //* possible race condition (exit state cannot be changed, problem can be ignored)
        while (env->getEnvironmentUpdateState() != EnvironmentState::EXITED) {
            // sleep_for(50ms);
            std::this_thread::yield(); //? sleep_for should be used
        }
        delete env;
        env = nullptr;
    }
    //
    // OutputBuffer
    OutputBuffer::OutputBuffer(std::ofstream& _logFile) :logFile(_logFile) {
        blocked = 0;
        waitingThread = 0;
    }
    OutputBuffer& OutputBuffer::operator<<(std::string str) {
        ++waitingThread;
        std::thread outThread = std::thread(outputStr, str);
        outThread.detach();
        return *this;
    }
    void OutputBuffer::outputStr(std::string str) {
        //? performance impact
        while (blocked) {
            // sleep_for(50ns);
            std::this_thread::yield();
        }
        blocked = true;
        std::cout << str;
        std::cout.flush();
        logFile << str;
        blocked = false;
        --waitingThread;
    }
    OutputBuffer::~OutputBuffer() {
        //? performance impact
        while (waitingThread > 0)
            // sleep_for(50ns);
            std::this_thread::yield();
    }
}