#ifndef NEATCPP_OUTPUTBUFFER_HEADER_H
#define NEATCPP_OUTPUTBUFFER_HEADER_H
#include <string>
#include <mutex>
#include <vector>
#include <atomic>
namespace neatCpp {
    enum OutputType { VERBOSE, INFO, WARNING, ERROR };
    struct Log {
        int id;
        std::string data;
        OutputType type;
    };
    // hold up to 50 lines
    class OutputBuffer {
    public:
        OutputBuffer(std::mutex& bufferUpdateMutex);
        void printLine(std::string str, OutputType type);
        const std::vector<Log>& getLogBuffer_unlocked() const;
        const std::vector<Log>& getLogBuffer_locked() const;
        bool isEmpty(int _flushedLogId) const;
        void flushed(int _flushedLogId);
    private:
        const int maxLogBufferSize = 200;
        const int logBufferReserveSize = 50;
        void appendBuffer(std::string str, OutputType type);
        std::vector<Log> logBuffer;
        std::mutex& bufferMutex;
        std::atomic<int> awaitingThread;
        std::atomic<int> nextLogId;
        std::atomic<int> flushedLogId;
    };
}
#endif