#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include "experiment.h"
#include "file_input.h"
#include "data_processor.h"
#include "result_generator.h"
using namespace data;
using namespace report;

/**
 *  Controls the workflow of the threads with DataProcessor instances.
 *  Runs the final data merge, calculations and displaying the results.
 */
namespace analyzer {
    const std::string OUTPUT_FILE_NAME{"../data/output_result.csv"};

    class TemperatureAnalyzer {
    public:
        TemperatureAnalyzer(const std::string &input_file_path,
                            size_t num_threads, size_t buffer_size_for_chunk);

        ~TemperatureAnalyzer() = default;

        void analyze();

        void displayTemperatureStatistics();

    private:
        size_t num_threads_;
        FileInput file_input_;
        DataProcessor data_processor_;
        std::vector<DataProcessor> processors_;
    };
}