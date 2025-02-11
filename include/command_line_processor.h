#pragma once
#include <string>
#include <fstream>
#include <istream>
#include <sstream>
#include <iostream>
#include "../include/data_file_generator.h"

/**
 * Processes arguments from the command line as well as makes input file checking.
 */
class CommandlineProcessor {
public:
    CommandlineProcessor(const std::string &inputFileName);
    bool processArguments(int argc, char* argv[]);
private:
    bool runHelpCommand();
    bool runGenerateCommand(std::istringstream &iss);

    std::string inputFileName;
};
