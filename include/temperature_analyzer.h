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

/**
 *  Controls the workflow of the threads with DataProcessor instances.
 *  Runs the final data merge, calculations and displaying the results.
 */
class TemperatureAnalyzer {
public:
    TemperatureAnalyzer(const std::string &inputFilePath, int numThreads, int bufferSizeForChunk);

    ~TemperatureAnalyzer();

    void analyze();

    void displayTemperatureStatistics();

    static const std::string &getOutputFileName();

private:
    int numThreads;
    FileInput fileInput;
    std::vector<DataProcessor> processors;
    static const std::string outputFileName;
};
