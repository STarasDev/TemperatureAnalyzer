#pragma once

#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

/**
 * Reads an input data file, and provides an access to its content with chunks, and implements the producer-consumer pattern.
 */
class FileInput {
public:
    FileInput(const std::string &filename, size_t bufferSize);

    ~FileInput();

    void initFileReading(const std::string &filename, size_t bufferSize);

    bool isEOF() const;

    std::vector<std::string> readChunk();

private:
    void readFile();

    std::ifstream file;
    const size_t bufferSize;
    std::vector<std::string> buffer;
    std::queue<std::vector<std::string>> chunks;
    std::thread readerThread;
    std::mutex mutex;
    std::condition_variable cv;
    bool endOfFile;
};