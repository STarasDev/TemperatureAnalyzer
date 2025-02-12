#include "../include/command_line_processor.h"
#include "../include/temperature_analyzer.h"
#include <chrono>
using namespace std::chrono;
using namespace command;

int main(int argc, char *argv[]) {
    // The value varies from 1 to std::thread::hardware_concurrency()
    const size_t num_threads = std::thread::hardware_concurrency();

    // A buffer size is a number of lines read as one chunk for processing
    constexpr size_t buffer_size = 5000;

    // Input file path
    const std::string input_file_name = "../data/input.csv";

    std::cout << "num_threads = "
              << num_threads
              << ", buffer_size = "
              << buffer_size
              << ", input_file_name = "
              << input_file_name
              << std::endl;

    CommandlineProcessor cmdProcessor(input_file_name);
    if (!cmdProcessor.processArguments(argc, argv)) {
        return 1;
    }

    auto start = high_resolution_clock::now();

    TemperatureAnalyzer analyzer(input_file_name, num_threads, buffer_size);
    analyzer.analyze();
    analyzer.displayTemperatureStatistics();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start).count();

    std::cout << "Time taken by the analyzer: "
              << duration << " milliseconds" << std::endl;

    return 0;
}
