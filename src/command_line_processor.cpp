#include <utility>

#include "../include/command_line_processor.h"

namespace command {
    CommandlineProcessor::CommandlineProcessor(std::string input_file_name)
        : input_file_name_(std::move(input_file_name)) {
    }

    /**
     * It implements a 'generate' command, which is used for input file creation.
     * @param argc
     * @param argv
     * @return
     */
    bool CommandlineProcessor::processArguments(int argc, char *argv[]) {
        std::ifstream input_file(input_file_name_);
        if (!input_file.good()) {
            std::cerr << "Please, provide the input file. The following file is not found: " << input_file_name_;
            std::cerr <<
                    ". As an alternative, you could use the 'generate=NUM_EXPERIMENTS,NUM_PROCESSES' command line parameter in order "
                    "to generate an input data file. The path will be used from the previous output (see the inputFile value)"
                    << std::endl;
            return false;
        }
        input_file.close();

        if (argc > 1) {
            std::string first_parameter;
            std::istringstream iss(argv[1]);
            std::getline(iss, first_parameter, '=');
            if (first_parameter == "generate") {
                return runGenerateCommand(iss);
            } else if (first_parameter == "help") {
                return runHelpCommand();
            }
        }
        return true;
    }

    bool CommandlineProcessor::runHelpCommand() {
        std::cout << "The TemperatureAnalyzer solution could be run with following arguments:"
                << std::endl
                << " help -- shows this message,"
                << std::endl
                << " generate=experiments_number,processes_number -- creates the input CSV file with"
                << std::endl
                << " experiments_number as numbers of experiments and processes_number as number of processes"
                << std::endl
                << "Example: ./TemperatureAnalyzer generate=1000,5"
                << std::endl;
        return false;
    }

    bool CommandlineProcessor::runGenerateCommand(std::istringstream &iss) const {
        std::string number_experiments_input, number_processes_input;
        std::getline(iss, number_experiments_input, ',');
        std::getline(iss, number_processes_input, ',');
        std::cout << "number_experiments_input="
                << number_experiments_input << ", number_processes_input="
                << number_processes_input << std::endl;
        size_t number_experiments = 0;
        size_t number_processes = 0;

        try {
            number_experiments = std::stoi(number_experiments_input);
            number_processes = std::stoi(number_processes_input);
        } catch (const std::exception &exception) {
            std::cerr << "Parameters error occurred. Please check your input: "
                    << exception.what() << std::endl;
            return false;
        }
        if (number_experiments <= 0
            || number_processes <= 0
            || number_experiments > 10000000
            || number_processes > 10000000) {
            std::cerr << "You have selected the generate file mode. In order to use it, please provide the correct "
                    "parameters, which shall be as follows: number_experiments > 0 "
                    "and number_experiments <= 10000000, number_processes > 0 and number_processes <= 10000000"
                    << std::endl;
            return false;
        }
        std::cout << "The input file will be generated now with the following parameters: number_experiments="
                << number_experiments
                << ", number_processes=" << number_processes << std::endl;

        DataFileGenerator::generate(input_file_name_, number_experiments, number_processes);
        return true;
    }
}
