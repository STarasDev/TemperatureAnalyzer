#include "temperature_analyzer.h"

namespace analyzer {
    /**
     * Constructor
     * @param input_file_path
     * @param num_threads
     * @param buffer_size_for_chunk
     */
    TemperatureAnalyzer::TemperatureAnalyzer(const std::string &input_file_path,
                                             size_t num_threads, size_t buffer_size_for_chunk)
        : num_threads_(num_threads),
          file_input_(input_file_path, buffer_size_for_chunk),
          processors_(num_threads) {
    }

    void TemperatureAnalyzer::analyze() {
        DataProcessor::removeTemporaryDataFiles();

        // Launch processing threads
        std::vector<std::thread> threads;
        for (size_t i = 0; i < num_threads_; ++i) {
            threads.emplace_back([this, i]() {
                int chunkId = 0;
                while (!file_input_.isEOF()) {
                    std::vector<std::string> chunk = file_input_.readChunk();
                    processors_[i].processChunk(chunk, i, chunkId);
                    ++chunkId;
                }
            });
        }

        // Wait for all threads to finish
        for (auto &thread: threads) {
            thread.join();
        }
    }

    /**
     * Runs the final data merge, calculations and displaying the results
     */
    void TemperatureAnalyzer::displayTemperatureStatistics() const {
        // Merge the results from all processors
        DataProcessor finalProcessor;
        for (const auto &processor: processors_) {
            finalProcessor.merge(processor);
        }
        // Print the results
        ResultGenerator::writeToConsole(finalProcessor);
        ResultGenerator::writeToFile(finalProcessor, OUTPUT_FILE_NAME);
    }
}
