#include "../include/command_line_processor.h"

CommandlineProcessor::CommandlineProcessor(const std::string &inputFileName)
: inputFileName(inputFileName) {
}

/**
 * It implements a 'generate' command, which is used for input file creation.
 * @param argc
 * @param argv
 * @return
 */
bool CommandlineProcessor::processArguments(int argc, char* argv[]) {
    std::ifstream inputFile(inputFileName);
    if(!inputFile.good()) {
        std::cerr << "Please, provide the input file. The following file is not found: " << inputFileName;
        std::cerr << ". As an alternative, you could use the 'generate=NUM_EXPERIMENTS,NUM_PROCESSES' command line parameter in order "
                     "to generate an input data file. The path will be used from the previous output (see the inputFile value)"
                     << std::endl;
        return false;
    }
    inputFile.close();

    if(argc > 1) {
        std::string firstParameter;
        std::istringstream iss(argv[1]);
        std::getline(iss, firstParameter, '=');
        if(firstParameter == "generate") {
            return runGenerateCommand(iss);
        } else if (firstParameter == "help") {
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
              << " generate=experimentsNum,processesNum -- creates the input CSV file with"
              << std::endl
              << " experimentsNum as numbers of experiments and processesNum as number of processes"
              << std::endl
              << "Example: ./TemperatureAnalyzer generate=1000,5"
              << std::endl;
    return false;
}

bool CommandlineProcessor::runGenerateCommand(std::istringstream &iss) {
    std::string experimentsNum, processesNum;
    std::getline(iss, experimentsNum, ',');
    std::getline(iss, processesNum, ',');
    std::cout << "experimentsNum="
    << experimentsNum << ", processesNum="
    << processesNum <<  std::endl;
    int numberExperiments = 0;
    int numberProcesses   = 0;

    try {
        numberExperiments = std::stoi(experimentsNum);
        numberProcesses = std::stoi(processesNum);
    } catch (const std::exception & exception) {
        std::cerr << "Parameters error occurred. Please check your input: "
        << exception.what() << std::endl;
        return false;
    }
    if(numberExperiments <= 0
    || numberProcesses <= 0
    || numberExperiments > 10000000
    || numberProcesses > 10000000) {
        std::cerr << "You have selected the generate file mode. In order to use it, please provide the correct "
                     "parameters, which shall be as follows: numberExperiments > 0 "
                     "and numberExperiments <= 10000000, numberProcesses > 0 and numberProcesses <= 10000000"
                     << std::endl;
        return false;
    }
    std::cout << "The input file will be generated now with the following parameters: numberExperiments="
    << numberExperiments
    << ", numberProcesses=" << numberProcesses <<  std::endl;

    DataFileGenerator generator;
    generator.generate(inputFileName, numberExperiments, numberProcesses);
    return true;
}
