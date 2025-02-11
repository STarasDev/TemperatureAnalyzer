#include "../include/command_line_processor.h"
#include "../include/temperature_analyzer.h"
#include <chrono>
using namespace std::chrono;

int main(int argc, char *argv[]) {
    // The value varies from 1 to std::thread::hardware_concurrency()
    const int numThreads = std::thread::hardware_concurrency();

    // A buffer size is a number of lines read as one chunk for processing
    const int bufferSize = 5000;

    // Input file path
    const std::string inputFileName = "../data/input.csv";

    std::cout << "numThreads = "
              << numThreads
              << ", bufferSize = "
              << bufferSize
              << ", inputFile = "
              << inputFileName
              << ", outputFile = "
              << TemperatureAnalyzer::getOutputFileName()
              << std::endl;

    CommandlineProcessor cmdProcessor(inputFileName);
    if (!cmdProcessor.processArguments(argc, argv)) {
        return 1;
    }

    auto start = high_resolution_clock::now();

    TemperatureAnalyzer analyzer(inputFileName, numThreads, bufferSize);
    analyzer.analyze();
    analyzer.displayTemperatureStatistics();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    std::cout << "Time taken by the analyzer: "
              << duration.count() / 1000.0
              << " seconds" << std::endl;

    return 0;
}
