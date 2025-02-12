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
namespace analyzer {
    const std::string DATA_FILES_DIRECTORY = "./";

    class DataProcessor {
    public:
        DataProcessor();

        void processChunk(const std::vector<std::string> &chunk, size_t thread_id, size_t chunk_id);

        void merge(const DataProcessor &other);

        const std::unordered_map<std::string, std::string>& getProcessNames() const;

        static std::unique_ptr<std::vector<double>>
        getProcessTemperaturesFromFile(const std::string &process_file_name) ;

        static void removeTemporaryDataFiles();

        const std::string& getExperimentNameWithHighestTemperature() const;
        double getExperimentHighestTemperature() const;

    private:
        void processExperiment(const std::string &name, const std::vector<double> &temperatures);

        void storeProcessExperiment(size_t thread_id, size_t chunk_id);

        std::unordered_map<std::string, std::vector<double>> process_temperatures_;
        std::unordered_map<std::string, std::string> process_names_;
        mutable std::mutex mutex_;

        std::string experiment_name_with_highest_temperature_;
        double highest_temperature_;
    };
}