#include "result_generator.h"

/**
 * Writes the results to the output file
 * @param processor
 */
void ResultGenerator::writeToFile(DataProcessor &processor, const std::string &outputFilename) {
    try {
        std::ofstream outputFile(outputFilename);
        if (!outputFile) {
            throw std::runtime_error("Failed to open output file: " + outputFilename);
        }

        calculateAndWriteResults(processor, outputFile);

        outputFile.close();
    } catch (const std::exception &ex) {
        std::cerr << "Error writing results to output file: " << ex.what() << std::endl;
    }
}

/**
 * Writes the results to the output file
 * @param processor
 */
void ResultGenerator::writeToConsole(DataProcessor &processor) {
    calculateAndWriteResults(processor, std::cout);
}

/**
 * Calculates results and writes them to the given stream
 * @param processor
 * @param os
 */
void ResultGenerator::calculateAndWriteResults(DataProcessor &processor, std::ostream &os) {
    os << "Process Name,Average Temperature,Highest Temperature,Lowest Temperature" << std::endl;

    for (const auto &entry: processor.getProcessNames()) {
        const std::string &processName = entry.first;
        const auto &temperatures = processor.getProcessTemperaturesFromFile(entry.second);
        double sum = 0.0;
        double lowest = (*temperatures)[0];
        double highest = (*temperatures)[0];
        for (double temperature: *temperatures) {
            sum += temperature;
            lowest = std::min(lowest, temperature);
            highest = std::max(highest, temperature);
        }

        double average = sum / temperatures->size();
        os << processName
           << "," << average
           << "," << lowest
           << "," << highest
           << std::endl;
    }

    os << "Process with the highest temperature: "
       << DataProcessor::getExperimentNameWithHighestTemperature()
       << ", temperature: " << DataProcessor::getExperimentHighestTemperature()
       << std::endl;
}
