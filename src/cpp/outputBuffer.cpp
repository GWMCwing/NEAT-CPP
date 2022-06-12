#include <thread>
#include "./header/outputBuffer.h"
namespace neatCpp {
    OutputBuffer::OutputBuffer(std::mutex& bufferUpdateMutex) : bufferMutex(bufferUpdateMutex) {
        nextLogId = 1;
        flushedLogId = 0;
        logBuffer.reserve(logBufferReserveSize);
    }
    void OutputBuffer::printLine(std::string str, OutputType type) {
        ++awaitingThread;
        std::thread appendThread = std::thread(appendBuffer, str, type);
        appendThread.detach();
    }
    void OutputBuffer::appendBuffer(std::string str, OutputType type) {
        std::lock_guard<std::mutex> lock(bufferMutex);
        logBuffer.push_back({ nextLogId, str, type });
        ++nextLogId;
        if (logBuffer.size() > maxLogBufferSize) {
            logBuffer.erase(logBuffer.begin(), logBuffer.begin() + 150);
        }
        --awaitingThread;
    }
    const std::vector<Log>& OutputBuffer::getLogBuffer_unlocked() const {
        std::lock_guard<std::mutex> lock(bufferMutex);
        return logBuffer;
    }
    const std::vector<Log>& OutputBuffer::getLogBuffer_locked() const { return logBuffer; }
    void OutputBuffer::flushed(int _flushedLogId) { flushedLogId = _flushedLogId; }
    bool OutputBuffer::isEmpty(int _flushedLogId) const { return !awaitingThread && flushedLogId == _flushedLogId; }
}