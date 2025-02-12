#include "../include/data_file_generator.h"

namespace data {
    /**
     * Generates the CSV data file with random temperatures for given experiments and processes
     * @param file_name
     * @param num_experiments
     * @param num_processes
     */
    void DataFileGenerator::generate(const std::string &file_name, size_t num_experiments, size_t num_processes) {
        std::ofstream output_file(file_name);

        // Generate header line
        output_file << "Experiment Name,Experiment ID";
        for (int i = 1; i <= num_processes; ++i) {
            output_file << ",P" << i << " Temperature";
        }
        output_file << '\n';

        // Generate data lines
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> temperatureDist(19.0, 81.0);

        for (int i = 1; i <= num_experiments; ++i) {
            output_file << "Experiment " << i << ',' << i;
            for (int j = 1; j <= num_experiments; ++j) {
                output_file << ',' << temperatureDist(gen);
            }
            output_file << '\n';
        }

        output_file.close();
    }
}
