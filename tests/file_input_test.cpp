#include "gtest/gtest.h"
#include "../include/file_input.h"

TEST(FileInputTest, ReadChunk)
{
    const size_t CHUNKS_SIZE = 6;
    std::vector<std::string> chunks[CHUNKS_SIZE];
    FileInput fileInput("../../data/test_input.csv", 2);
    for(size_t index = 0 ; index < CHUNKS_SIZE ; ++index)
    {
        chunks[index] = fileInput.readChunk();
    }

    ASSERT_EQ(chunks[0].size(), 2);
    ASSERT_EQ(chunks[1].size(), 2);
    ASSERT_EQ(chunks[2].size(), 2);
    ASSERT_TRUE(chunks[3].empty());
}

