#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include "../include/data_file_generator.h"
using namespace data;

/**
 * Processes arguments from the command line as well as makes input file checking.
 */
namespace command {
    class CommandlineProcessor {
    public:
        explicit CommandlineProcessor(std::string input_file_name);

        bool processArguments(int argc, char *argv[]);

    private:
        static bool runHelpCommand();

        bool runGenerateCommand(std::istringstream &iss) const;

        std::string input_file_name_;
    };
}
