#include "../include/data_file_generator.h"

/**
 * Generates the CSV data file with random temperatures for given experiments and processes
 * @param filename
 * @param numExperiments
 * @param numProcesses
 */
void DataFileGenerator::generate(const std::string &filename, int numExperiments, int numProcesses) {
    std::ofstream outputFile(filename);

    // Generate header line
    outputFile << "Experiment Name,Experiment ID";
    for (int i = 1; i <= numProcesses; ++i) {
        outputFile << ",P" << i << " Temperature";
    }
    outputFile << '\n';

    // Generate data lines
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> temperatureDist(19.0, 81.0);

    for (int i = 1; i <= numExperiments; ++i) {
        outputFile << "Experiment " << i << ',' << i;
        for (int j = 1; j <= numProcesses; ++j) {
            outputFile << ',' << temperatureDist(gen);
        }
        outputFile << '\n';
    }

    outputFile.close();
}
