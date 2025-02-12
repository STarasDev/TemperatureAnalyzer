#include "../include/file_input.h"

namespace data {
    /**
     * FileInput constructor
     * @param file_name
     * @param buffer_size
     */
    FileInput::FileInput(const std::string &file_name, size_t buffer_size)
            : buffer_size_{buffer_size}, end_of_file_{false} {
        initFileReading(file_name);
    }

    /**
     * Initialises a file reading in a thread
     * @param file_name
     */
    void FileInput::initFileReading(const std::string &file_name) {
        file_.open(file_name);
        if (!file_.is_open()) {
            throw std::runtime_error("Failed to open file: " + file_name);
        }

        reader_thread_ = std::thread(&FileInput::readFile, this);
    }

    /**
     * Destructor
     */
    FileInput::~FileInput() {
        if (reader_thread_.joinable()) {
            reader_thread_.join();
        }
        file_.close();
    }

    /**
     * Checks if a file reading is finished and there are no data to be processed
     * @return
     */
    bool FileInput::isEOF() const {
        return end_of_file_ && chunks_.empty();
    }

    /**
     * Reads a chunk of file and gives it to a reading thread when it's ready
     * @return
     */
    std::vector<std::string> FileInput::readChunk() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [&] { return !chunks_.empty() || end_of_file_; });
        if (!chunks_.empty()) {
            std::vector<std::string> chunk = std::move(chunks_.front());
            chunks_.pop();
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
        while (std::getline(file_, line)) {
            std::unique_lock<std::mutex> lock(mutex_);
            if (!headerSkipped) {
                headerSkipped = true;
                continue;
            }
            buffer_.push_back(line);
            if (buffer_.size() >= buffer_size_) {
                chunks_.push(std::move(buffer_));
                buffer_.clear();
                cv_.notify_one();
            }
        }

        if (!buffer_.empty()) {
            chunks_.push(std::move(buffer_));
            buffer_.clear();
        }
        end_of_file_ = true;
        cv_.notify_all();
    }
}