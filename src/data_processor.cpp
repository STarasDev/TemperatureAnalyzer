#include "../include/data_processor.h"

const std::string DataProcessor::dataFilesDirectory = "./";
std::string DataProcessor::experimentNameWithHighestTemperature = "";
double DataProcessor::highestTemperature = 0.0;

/**
 * Processes a chunk, i.e. a portion ot the input file
 * @param chunk
 * @param threadId
 * @param chunkId
 */
void DataProcessor::processChunk(const std::vector<std::string> &chunk, int threadId, int chunkId) {
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

        processExperiment(name, id, temperatures);
    }
    storeProcessExperiment(threadId, chunkId);
}

/**
 * Processes the experiment's values. This method is being used by an external thread
 * @param name
 * @param id
 * @param temperatures
 */
void DataProcessor::processExperiment(const std::string &name, const std::string &id,
                                      const std::vector<double> &temperatures) {
    std::lock_guard<std::mutex> lock(mutex);

    // Update the average temperature for each process
    // Update the experiment name with the highest temperature
    size_t numProcesses = temperatures.size();
    for (size_t i = 0; i < numProcesses; ++i) {
        if (highestTemperature < temperatures[i]) {
            highestTemperature = temperatures[i];
            experimentNameWithHighestTemperature = name;
        }
        std::string processName = "P" + std::to_string(i + 1);
        processTemperatures[processName].push_back(temperatures[i]);
    }
}

/**
 * Merges the names of other processors, which are being used for process files reading
 * @param other
 */
void DataProcessor::merge(const DataProcessor &other) {
    for (const auto &entry: other.processNames) {
        processNames.emplace(entry);
    }
}

/**
 * Stores the process' temperatures in a file for the current thread and a chunk Id
 * @param threadId
 * @param chunkId
 */
void DataProcessor::storeProcessExperiment(int threadId, int chunkId) {
    std::lock_guard<std::mutex> lock(mutex);

    for (const auto &aProcessTemperatures: processTemperatures) {

        const std::string &processName = aProcessTemperatures.first;
        const std::string fileName = dataFilesDirectory + "process_" + processName + "_"
                                     + std::to_string(threadId) + "_"
                                     + std::to_string(chunkId) + ".dat";

        processNames.emplace(processName, fileName);

        std::ofstream outputFile(fileName,  std::ios::binary | std::ios::ate | std::ios_base::app);
        for (const auto &temperature: aProcessTemperatures.second) {
            std::string temp = std::to_string(temperature) + "\n";
            outputFile.write(temp.c_str(), temp.size());
        }
        outputFile.close();
    }

    processTemperatures.clear();
}

/**
 * Loads the process' temperatures from a file, which corresponds a chunk
 * @param processFileName
 * @return
 */
const std::unique_ptr<std::vector<double>>
DataProcessor::getProcessTemperaturesFromFile(const std::string &processFileName) const {
    std::ifstream processFile(processFileName);
    std::string line;
    std::unique_ptr<std::vector<double>> processTemperatures(new std::vector<double>());
    while (std::getline(processFile, line)) {
        processTemperatures.get()->emplace_back(std::stod(line));
    }
    return processTemperatures;
}

/**
 * Gets the process' names needed to read chunk files
 * @return
 */
const std::unordered_map<std::string, std::string> DataProcessor::getProcessNames() {
    return processNames;
}

const std::string& DataProcessor::getExperimentNameWithHighestTemperature() {
    return experimentNameWithHighestTemperature;
}

double DataProcessor::getExperimentHighestTemperature() {
    return highestTemperature;;
}

const std::string& DataProcessor::getDataFilesDirectory() {
    return dataFilesDirectory;
}

/**
 * Deletes the temporary *.dat files, which are used to accumulate results during calculations
 */
void DataProcessor::removeTemporaryDataFiles() {
    const std::string &directory = dataFilesDirectory;
    const std::string &fileMask = ".dat";
    DIR *dir = opendir(directory.c_str());
    if (dir == nullptr) {
        throw std::runtime_error("Failed to open directory: " + directory);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string fileName = entry->d_name;
        if (fileName.find(fileMask) != std::string::npos) {
            std::string filePath = directory + "/" + fileName;
            struct stat fileStat;
            if (stat(filePath.c_str(), &fileStat) == 0 && S_ISREG(fileStat.st_mode)) {
                if (remove(filePath.c_str()) != 0) {
                    throw std::runtime_error("Failed to remove file: " + filePath);
                }
            }
        }
    }

    closedir(dir);
}
