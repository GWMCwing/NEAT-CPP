#ifndef NEATCPP_OUTPUTBUFFER_HEADER_H
#define NEATCPP_OUTPUTBUFFER_HEADER_H
#include <string>
#include <mutex>
#include <vector>
#include <atomic>
namespace neatCpp {
    // enum for specifying type of output 
    enum OutputType { VERBOSE, INFO, WARNING, ERROR };
    // struct for passing log string with additional information and identifier id
    struct Log {
        int id;
        std::string data;
        OutputType type;
    };
    // hold up to 50 lines
    class OutputBuffer {
    public:
        /**
         * @brief Construct a new Output Buffer object
         *
         * @param bufferUpdateMutex reference of a mutex provided from it's parent object (Object that is containing it) for access controlling
         */
        OutputBuffer(std::mutex& bufferUpdateMutex);
        /**
         * @brief async method for pushing a string to buffer
         *
         * @param str string to be buffered
         * @param type output type of the string
         */
        void printLine(std::string str, OutputType type);
        /**
         * @brief Get the vector of string in buffer, lock guard is not performed
         *
         * @return const std::vector<Log>& vector of string in log buffer
         */
        const std::vector<Log> getLogBuffer_locked() const;
        /**
         * @brief Get the vector of string in buffer, lock guard is performed
         *
         * @return const std::vector<Log>& vector of string in log buffer
         */
        const std::vector<Log> getLogBuffer_unlocked() const;
        /**
         * @brief return true if the buffer is empty , awaiting thread is not checked
         *
         * @param _flushedLogId
         * @return true if both buffer, false otherwise
         */
        bool isEmpty(int _flushedLogId) const;
        /**
         * @brief update flushed id, used by cli
         *
         * @param _flushedLogId id to be updated to
         */
        void flushed(int _flushedLogId);
    private:
        // control the size of log vector in buffer to trigger a resize
        const int maxLogBufferSize = 200;
        // default resize size of buffer vector
        const int logBufferReserveSize = 50;
        /**
         * @brief push the string to buffer, should be used as asynchronous function call
         *
         * @param str string to be pushed
         * @param type output type of the string
         */
        void pushToBuffer(std::string str, OutputType type);
        // buffer for log object to be pushed or flushed to log
        std::vector<Log> logBuffer;
        // mutex lock for ofstream flushing and vector pushing
        std::mutex& bufferMutex;
        // store number of thread awaiting to push to the vector string
        std::atomic<int> awaitingThread;
        // store the id to be used for next log object for pushing
        std::atomic<int> nextLogId;
        // store the id of flushed log
        std::atomic<int> flushedLogId;
    };
}
#endif