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
namespace data {
    class FileInput {
    public:
        FileInput(const std::string &file_name, size_t buffer_size);

        ~FileInput();

        void initFileReading(const std::string &file_name);

        bool isEOF() const;

        std::vector<std::string> readChunk();

    private:
        void readFile();

        std::ifstream file_;
        const size_t buffer_size_;
        std::vector<std::string> buffer_;
        std::queue<std::vector<std::string>> chunks_;
        std::thread reader_thread_;
        std::mutex mutex_;
        std::condition_variable cv_;
        bool end_of_file_;
    };
}