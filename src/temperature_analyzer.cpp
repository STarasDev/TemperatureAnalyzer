#include "temperature_analyzer.h"

const std::string TemperatureAnalyzer::outputFileName = "../data/output_result.csv";

/**
 * Constructor
 * @param inputFilePath
 * @param numThreads
 * @param bufferSizeForChunk
 */
TemperatureAnalyzer::TemperatureAnalyzer(const std::string &inputFilePath,
                                         int numThreads, int bufferSizeForChunk)
        : numThreads(numThreads),
          fileInput(inputFilePath, bufferSizeForChunk),
          processors(numThreads) {
}

/**
 * Destructor
 */
TemperatureAnalyzer::~TemperatureAnalyzer() {
}

void TemperatureAnalyzer::analyze() {
    DataProcessor::removeTemporaryDataFiles();
    // Launch processing threads
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([this, i]() {
            int chunkId = 0;
            while (!fileInput.isEOF()) {
                std::vector<std::string> chunk = fileInput.readChunk();
                processors[i].processChunk(chunk, i, chunkId);
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
void TemperatureAnalyzer::displayTemperatureStatistics() {
    // Merge the results from all processors
    DataProcessor finalProcessor;
    for (const auto &processor: processors) {
        finalProcessor.merge(processor);
    }
    // Print the results
    ResultGenerator::writeToConsole(finalProcessor);
    ResultGenerator::writeToFile(finalProcessor, outputFileName);
}

const std::string &TemperatureAnalyzer::getOutputFileName() {
    return outputFileName;
}
