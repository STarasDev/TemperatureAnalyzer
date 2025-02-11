#pragma once

#include <ostream>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "../include/data_processor.h"

/**
 * Calculates and writes results to a given output
 */
class ResultGenerator {
public:
    static void writeToConsole(DataProcessor &processor);

    static void writeToFile(DataProcessor &processor, const std::string &outputFilename);

    static void calculateAndWriteResults(DataProcessor &processor, std::ostream &os);
};
