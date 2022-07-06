#include <ctime>
#include <iostream>

#include "./cli.h"

namespace neatCpp {
    class Environment;
    using namespace std::this_thread;     // sleep_for, sleep_until
    using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
    using std::chrono::system_clock;
    CLI::CLI(int populationSize, int inputSize, int outputSize, int _expectedGeneration, std::ofstream& logFileStream) : logFile(logFileStream) {
        // \033[=7l to disable line wrapping?
        outputBuffer = new OutputBuffer(bufferUpdateMutex);
        //! do NOT lock, dead lock std::lock_guard<std::mutex> lock(envUpdateMutex);
        env = new Environment(populationSize, inputSize, outputSize, outputBuffer, envUpdateMutex);
        //
        previousBestScore = 0;
        previousAverageScore = 0;
        expectedGeneration = _expectedGeneration;
        // display info
        infoBlockLine = 1;
        infoBlockHeight = 4;
        logStartLine = 6;
        logHeight = 8;
        inputLine = 14;
        flushedLogId = 0;
        startTime = time(nullptr);
        updateDisplay();
        startContinuousUpdate(); // create new thread for continuous update
    }
    CLI::~CLI() {
        awaitEnvExit();
        delete env;
        awaitBufferClear();
        delete outputBuffer;
    }
    void CLI::flushToLogFile() {
        const std::lock_guard<std::mutex> lock(bufferUpdateMutex);
        const std::vector<Log> logBuffer = outputBuffer->getLogBuffer_unlocked();
        int startIndex = flushedLogId - logBuffer[0].id;
        if (startIndex < 0) startIndex = 0;
        for (int i = startIndex;i < logBuffer.size();++i) {
            logFile << formatString(logBuffer[i]) << "\n";
        }
        flushedLogId = logBuffer.back().id;
        outputBuffer->flushed(flushedLogId);
    }
    void CLI::updateDisplay() {

        // to prevent unexpected concurrent call (maybe encountered during user call)
        std::lock_guard<std::mutex> lockDisplay(displayUpdateMutex);
        std::lock_guard<std::mutex> lockBuffer(bufferUpdateMutex);
        //
        std::string durationStr, bestScoreStr, bestScoreDeltaStr, averageScoreStr, averageScoreDeltaStr;
        getDurationStr(durationStr);
        std::string generationStr = std::to_string(env->getGeneration());
        getBestScoreStr(bestScoreStr, bestScoreDeltaStr);
        getAverageScoreStr(averageScoreStr, averageScoreDeltaStr);
        // bestScoreStr = '0';
        // bestScoreDeltaStr = '0';
        // averageScoreDeltaStr = '0';
        // averageScoreStr = '0';

        //
        std::string envState = "EnvState";
        std::vector<Log> outputBufferLog = outputBuffer->getLogBuffer_unlocked();
        //?????
        int logStart = outputBufferLog.size() - logHeight;
        //
        std::cout << "\0337\033[3;1HDuration: " + durationStr + " Generation: " + generationStr + "\0338";
        std::cout << "\0337\033[4;1HBest Score: " + bestScoreStr + ", Delta: " + bestScoreDeltaStr + "\0338";
        std::cout << "\0337\033[5;1HAverage Score: " + averageScoreStr + ", Delta: " + averageScoreDeltaStr + "\0338";
        std::cout << "\0337\033[6;1HEnvironment State: " + envState + "\0338";
        std::cout << "\0337\033[7;1H---------------------\0338";
        if (logStart > 0)
            for (int i = 0; i < logHeight; ++i) {
                std::string logString = formatString_colored(outputBufferLog[logStart + i]);
                std::cout << "\0337\033[" + std::to_string(logStartLine + i + 2) + ";1H" + logString + "\0338";
            }
        std::cout.flush();
    }
    void CLI::getDurationStr(std::string& durationStr) const {
        std::time_t deltaTime = time(nullptr) - startTime;
        std::string hourStr = "0" + std::to_string((int)(deltaTime / 3600));
        std::string minuteStr = "0" + std::to_string((int)((deltaTime / 60) % 60));
        std::string secondStr = "0" + std::to_string((int)((deltaTime % 60)));
        durationStr = hourStr.substr(1) + ":" + minuteStr.substr(minuteStr.size() - 2) + ":" + secondStr.substr(secondStr.size() - 2);
    }
    void CLI::getAverageScoreStr(std::string& averageScoreStr, std::string& averageScoreDeltaStr) const {
        long double averageScore = env->getAverageScore();
        averageScoreStr = std::to_string(averageScore);
        averageScoreDeltaStr = std::to_string(averageScore - previousAverageScore);
        if (averageScoreStr.size() > maxNumberChar) averageScoreStr.substr(0, maxNumberChar);
        if (averageScoreDeltaStr.size() > maxNumberChar) averageScoreDeltaStr.substr(0, maxNumberChar);
    }
    void CLI::getBestScoreStr(std::string& bestScoreStr, std::string& bestScoreDeltaStr) const {
        long double bestScore = env->getBestScore();
        bestScoreStr = std::to_string(bestScore);
        bestScoreDeltaStr = std::to_string(bestScore - previousBestScore);
        if (bestScoreStr.size() > maxNumberChar) bestScoreStr.substr(0, maxNumberChar);
        if (bestScoreDeltaStr.size() > maxNumberChar) bestScoreDeltaStr.substr(0, maxNumberChar);
    }
    void CLI::handleUserInput() {
        //TODO
    }
    bool CLI::updateOnce() {
        if (envThreadStarted) return false;
        envThreadStarted = true;
        env->updateOnce();
        return true;
    }
    void CLI::exitCli() {
        stopEnv();
        awaitBufferClear();
    }
    void CLI::stopEnv() {
        env->exitEnv();
        awaitEnvExit();
    }
    void CLI::exportPopulation(std::fstream& exportStream) const {
        env->exportPopulation(exportStream);
    }
    void CLI::exportBestPlayer(std::fstream& exportStream) const {
        env->exportBestPlayer(exportStream);
    }
    void CLI::awaitEnvExit() {
        while (!env->isEnvExited()) {
            std::this_thread::yield();
        }
    }
    void CLI::awaitBufferClear() {
        while (!outputBuffer->isEmpty(flushedLogId)) {
            flushToLogFile();
            std::this_thread::yield();
        }
    }
    bool CLI::startContinuousUpdate() {
        if (envThreadStarted) return false;
        envThreadStarted = true;
        envThread = std::thread(&CLI::continuousUpdateEnvironment, this);
        envThread.detach();
        return true;
    }
    void CLI::continuousUpdateEnvironment() {
        while (!(env->isEnvExiting())) {
            env->updateOnce();
            updateDisplay();
            std::this_thread::sleep_for(500ms);
        }
    }
    std::string CLI::formatString(Log log) const {
        // https://en.cppreference.com/w/cpp/chrono/c/strftime
        char timeStr[20];
        std::time_t t = std::time(nullptr);
        std::strftime(timeStr, sizeof(timeStr), "%T", std::localtime(&t));
        std::string timeString = timeStr;
        std::string rStr = "[" + timeString + "] " + log.data;
        return rStr;
    }
    std::string CLI::formatString_colored(Log log) const {
        std::string colorEsc;
        std::string str = formatString(log);
        switch (log.type) {
            case OutputType::VERBOSE:
                colorEsc = "\033[38;5;8m"; // gary-ish
                break;
            case OutputType::INFO:
                colorEsc = "\033[39m"; // default
                break;
            case OutputType::WARNING:
                colorEsc = "\033[38;5;208m"; // orange-ish
                break;
            case OutputType::ERROR:
                colorEsc = "\033[91m"; // bright red
                break;
            default:
                colorEsc = "\033[39m"; // default
                break;
        }
        std::string rStr = colorEsc + str + "\033[39;49m";
        return rStr;
    }
}