#pragma once

#include <iostream>
#include <sstream>
#include "../include/data_processor.h"
using namespace analyzer;

/**
 * Calculates and writes results to a given output
 */
namespace report {
    class ResultGenerator {
    public:
        static void writeToConsole(const DataProcessor &processor);

        static void writeToFile(const DataProcessor &processor, const std::string &output_file_name);

        static void calculateAndWriteResults(const DataProcessor &processor, std::ostream &os);
    };
}