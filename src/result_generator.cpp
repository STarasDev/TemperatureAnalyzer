#include "result_generator.h"

namespace report {
    /**
     * Writes the results to the output file
     * @param processor
     */
    void ResultGenerator::writeToFile(const DataProcessor &processor, const std::string &output_file_name) {
        try {
            std::ofstream output_file(output_file_name);
            if (!output_file) {
                throw std::runtime_error("Failed to open output file: " + output_file_name);
            }

            calculateAndWriteResults(processor, output_file);

            output_file.close();
        } catch (const std::exception &ex) {
            std::cerr << "Error writing results to output file: " << ex.what() << std::endl;
        }
    }

    /**
     * Writes the results to the output file
     * @param processor
     */
    void ResultGenerator::writeToConsole(const DataProcessor &processor) {
        calculateAndWriteResults(processor, std::cout);
    }

    /**
     * Calculates results and writes them to the given stream
     * @param processor
     * @param os
     */
    void ResultGenerator::calculateAndWriteResults(const DataProcessor &processor, std::ostream &os) {
        os << "Process Name,Average Temperature,Highest Temperature,Lowest Temperature" << std::endl;

        for (const auto &entry: processor.getProcessNames()) {
            const std::string &process_name = entry.first;
            const auto &temperatures = DataProcessor::getProcessTemperaturesFromFile(entry.second);
            double sum = 0.0;
            double lowest = (*temperatures)[0];
            double highest = (*temperatures)[0];
            for (double temperature: *temperatures) {
                sum += temperature;
                lowest = std::min(lowest, temperature);
                highest = std::max(highest, temperature);
            }

            double average = sum / static_cast<double>(temperatures->size());
            os << process_name
               << "," << average
               << "," << lowest
               << "," << highest
               << std::endl;
        }

        os << "Process with the highest temperature: "
           << processor.getExperimentNameWithHighestTemperature()
           << ", temperature: " << processor.getExperimentHighestTemperature()
           << std::endl;
    }
}