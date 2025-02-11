#pragma once

#include <string>
#include <fstream>
#include <random>

/**
 * A utility class, which generates the CSV data file with random temperatures for given experiments and processes.
 */
class DataFileGenerator {
public:
    static void generate(const std::string &filename, int numExperiments, int numProcesses);
};