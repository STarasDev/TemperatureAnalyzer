#pragma once

#include <fstream>
#include <random>

/**
 * A utility class, which generates the CSV data file with random temperatures for given experiments and processes.
 */
namespace data {
    class DataFileGenerator {
    public:
        static void generate(const std::string &file_name, size_t num_experiments, size_t num_processes);
    };
}