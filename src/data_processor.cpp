#include "../include/data_processor.h"

namespace analyzer {
    /**
     * Constructor
     */
    DataProcessor::DataProcessor()
        : highest_temperature_{0.0} {
    }

    /**
     * Processes a chunk, i.e. a portion ot the input file
     * @param chunk
     * @param thread_id
     * @param chunk_id
     */
    void DataProcessor::processChunk(const std::vector<std::string> &chunk, size_t thread_id, size_t chunk_id) {
        for (const auto &line: chunk) {
            // Split the CSV line into individual values
            // Assuming the columns are: name, id, P1 temp, P2 temp, ..., Pn temp
            std::istringstream iss(line);
            std::string name, id;
            std::vector<double> temperatures;
            std::string value;

            std::getline(iss, name, ',');
            std::getline(iss, id, ',');
            while (std::getline(iss, value, ',')) {
                temperatures.push_back(std::stod(value));
            }

            processExperiment(name, temperatures);
        }
        storeProcessExperiment(thread_id, chunk_id);
    }

    /**
     * Processes the experiment's values. This method is being used by an external thread
     * @param name
     * @param temperatures
     */
    void DataProcessor::processExperiment(const std::string &name, const std::vector<double> &temperatures) {
        std::lock_guard<std::mutex> lock(mutex_);

        // Update the average temperature for each process
        // Update the experiment name with the highest temperature
        size_t num_processes = temperatures.size();
        for (size_t i = 0; i < num_processes; ++i) {
            if (highest_temperature_ < temperatures[i]) {
                highest_temperature_ = temperatures[i];
                experiment_name_with_highest_temperature_ = name;
            }
            std::string process_name = "P";
            process_name += std::to_string(i + 1);
            process_temperatures_[process_name].push_back(temperatures[i]);
        }
    }

    /**
     * Merges the names of other processors, which are being used for process files reading
     * @param other
     */
    void DataProcessor::merge(const DataProcessor &other) {
        for (const auto &entry: other.process_names_) {
            process_names_.emplace(entry);
            experiment_name_with_highest_temperature_ = other.getExperimentNameWithHighestTemperature();
            highest_temperature_ = other.getExperimentHighestTemperature();
        }
    }

    /**
     * Stores the process' temperatures in a file for the current thread and a chunk Id
     * @param thread_id
     * @param chunk_id
     */
    void DataProcessor::storeProcessExperiment(size_t thread_id, size_t chunk_id) {
        std::lock_guard<std::mutex> lock(mutex_);

        for (const auto &process_temperatures: process_temperatures_) {

            const std::string &process_name = process_temperatures.first;
            std::string file_name = DATA_FILES_DIRECTORY;
            file_name.append("process_").append(process_name)
            .append("_").append(std::to_string(thread_id))
            .append("_").append(std::to_string(chunk_id)).append(".dat");

            process_names_.emplace(process_name, file_name);

            std::ofstream output_file(file_name,  std::ios::binary | std::ios::ate | std::ios_base::app);
            for (const auto &temperature: process_temperatures.second) {
                std::string temp = std::to_string(temperature) + "\n";
                output_file.write(temp.c_str(), temp.size());
            }
            output_file.close();
        }

        process_temperatures_.clear();
    }

    /**
     * Loads the process' temperatures from a file, which corresponds a chunk
     * @param process_file_name
     * @return process_temperatures
     */
    std::unique_ptr<std::vector<double>>
    DataProcessor::getProcessTemperaturesFromFile(const std::string &process_file_name) {
        std::ifstream process_file(process_file_name);
        std::string line;
        std::unique_ptr<std::vector<double>> process_temperatures(new std::vector<double>());
        while (std::getline(process_file, line)) {
            process_temperatures->emplace_back(std::stod(line));
        }
        return process_temperatures;
    }

    /**
     * Gets the process' names needed to read chunk files
     * @return process_names_
     */
    const std::unordered_map<std::string, std::string>& DataProcessor::getProcessNames() const {
        return process_names_;
    }

    const std::string& DataProcessor::getExperimentNameWithHighestTemperature() const {
        return experiment_name_with_highest_temperature_;
    }

    double DataProcessor::getExperimentHighestTemperature() const {
        return highest_temperature_;
    }

    /**
     * Deletes the temporary *.dat files, which are used to accumulate results during calculations
     */
    void DataProcessor::removeTemporaryDataFiles() {
        const std::string file_mask{".dat"};
        DIR *dir = opendir(DATA_FILES_DIRECTORY.c_str());
        if (dir == nullptr) {
            throw std::runtime_error("Failed to open directory: " + DATA_FILES_DIRECTORY);
        }

        dirent *entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string file_name = entry->d_name;
            if (file_name.find(file_mask) != std::string::npos) {
                std::string file_path = DATA_FILES_DIRECTORY;
                file_path.append("/").append(file_name);

                struct stat file_stat{};
                if (stat(file_path.c_str(), &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
                    if (remove(file_path.c_str()) != 0) {
                        throw std::runtime_error("Failed to remove file: " + file_path);
                    }
                }
            }
        }

        closedir(dir);
    }
}