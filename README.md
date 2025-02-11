# TemperatureAnalyzer
Description: 

A program written in C++ that reads a dataset of experiment’s parameters from a file, processes it, and outputs the following:
- The average temperature for each process
- The highest and lowest temperature for each process
- The experiment with the highest overall temperature 

It uses distributed computing and multithreading to handle large datasets.

========= Building and running the solution =========

Run the following command withing the root directory of the project. It will build and run the solution:

mkdir build && cd build && cmake .. && make

The following command made from the root directory will run the solution:

cd build && ./TemperatureAnalyzer

The following command shows a simple help message:
cd build && ./TemperatureAnalyzer help

The following command generates the input CSV file:
cd build && ./TemperatureAnalyzer generate=1000,5

The following command made from the project's root directory will run the unit tests:

cd build/tests && ./data_processor_test_tests ; ./file_input_test_tests
