#include "config.hpp"
#include <fstream>

void Config::parseConfig(std::ifstream &f){

    f >> near >> far >> fovY >> ratio;
    f >> levelOfRecursion >> w;
    
}