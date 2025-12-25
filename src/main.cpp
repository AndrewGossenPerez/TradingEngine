
#include "data/csv_reader.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <chrono>

int main(){

    using clock = std::chrono::steady_clock;

    std::cout << "Creating bars" << std::endl;
    auto t0 = clock::now();
    auto bars=trd::csvReader::loadBars("samples/aapl.csv");
    auto t1 = clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    std::cout << " Bars created: " << bars.size() << 
    " Total Time to create : " << elapsed_ms << "ms" << std::endl;

    return 0;

}