#pragma once

#include "experiment.h"

#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <mutex>
#include <memory>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <sys/stat.h>

/**
 * A main working class, which processes a chink of data with experiments, and produces the statistics results
 * including the intermediary files with processes' data.
 */
class DataProcessor {
public:
    void processChunk(const std::vector<std::string> &chunk, int threadId, int chunkId);

    void merge(const DataProcessor &other);

    const std::unordered_map<std::string, std::string> getProcessNames();

    const std::unique_ptr<std::vector<double>>
    getProcessTemperaturesFromFile(const std::string &processFileName) const;

    static void removeTemporaryDataFiles();

    static const std::string& getExperimentNameWithHighestTemperature();
    static double getExperimentHighestTemperature();
    static const std::string& getDataFilesDirectory();

private:
    void processExperiment(const std::string &name, const std::string &id,
                           const std::vector<double> &temperatures);

    void storeProcessExperiment(int threadId, int chunkId);

    std::unordered_map<std::string, std::vector<double>> processTemperatures;
    std::unordered_map<std::string, std::string> processNames;
    mutable std::mutex mutex;

    static const std::string dataFilesDirectory;
    static const std::string fileNameHighestTemperature;

    static std::string experimentNameWithHighestTemperature;
    static double highestTemperature;
};
