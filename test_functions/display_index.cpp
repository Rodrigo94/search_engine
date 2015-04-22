#include "display_index.h"


void show_file(std::string filename, std::map<std::string, int>& vocabulary){
  std::ifstream file (filename.c_str(), std::ifstream::in | std::ifstream::binary);
  int buff;
  while(!file.eof()){
    file.read((char *)&(buff), sizeof(buff));
    std::cout << buff << ",";
    file.read((char *)&(buff), sizeof(buff));
    std::cout << buff << ",";
    file.read((char *)&(buff), sizeof(buff));
    std::cout << buff << ",";
    file.read((char *)&(buff), sizeof(buff));
    std::cout << buff << std::endl;
  }
  file.close();
}


