#include "gtest/gtest.h"
#include "../include/data_processor.h"
using namespace analyzer;

TEST(DataProcessorTest, ProcessChunk) {
    DataProcessor dataProcessor;
    std::vector<std::string> chunk = {
        "Experiment1,ID1,1.0,2.0,3.0",
        "Experiment2,ID2,4.0,5.0,6.0"
    };
    dataProcessor.processChunk(chunk, 0, 0);
}
