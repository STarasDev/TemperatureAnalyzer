#include "../include/file_input.h"

/**
 * FileInput constructor
 * @param filename
 * @param bufferSize
 */
FileInput::FileInput(const std::string &filename, size_t bufferSize)
        : bufferSize(bufferSize), endOfFile(false) {
    initFileReading(filename, bufferSize);
}

/**
 * Initialises a file reading in a thread
 * @param filename
 * @param bufferSize
 */
void FileInput::initFileReading(const std::string &filename, size_t bufferSize) {
    file.open(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    readerThread = std::thread(&FileInput::readFile, this);
}

/**
 * Destructor
 */
FileInput::~FileInput() {
    if (readerThread.joinable()) {
        readerThread.join();
    }
    file.close();
}

/**
 * Checks if a file reading is finished and there are no data to be processed
 * @return
 */
bool FileInput::isEOF() const {
    return endOfFile && chunks.empty();
}

/**
 * Reads a chunk of file and gives it to a reading thread when it's ready
 * @return
 */
std::vector<std::string> FileInput::readChunk() {
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, [&] { return !chunks.empty() || endOfFile; });
    if (!chunks.empty()) {
        std::vector<std::string> chunk = std::move(chunks.front());
        chunks.pop();
        return chunk;
    }
    return {};
}

/**
 * A file reading procedure. It is used by
 * a separate thread, which implements a producer-consumer pattern.
 */
void FileInput::readFile() {
    std::string line;
    bool headerSkipped = false;
    while (std::getline(file, line)) {
        std::unique_lock<std::mutex> lock(mutex);
        if (!headerSkipped) {
            headerSkipped = true;
            continue;
        }
        buffer.push_back(line);
        if (buffer.size() >= bufferSize) {
            chunks.push(std::move(buffer));
            buffer.clear();
            cv.notify_one();
        }
    }

    if (!buffer.empty()) {
        chunks.push(std::move(buffer));
        buffer.clear();
    }
    endOfFile = true;
    cv.notify_all();
}
