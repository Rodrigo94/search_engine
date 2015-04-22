#include "display_index.h"

void show_file(std::string filename){
  std::ifstream file (filename.c_str(), std::ifstream::in | std::ifstream::binary);
  int buff;
  for(int i=0; i<100; i++){
    file.read((char *)&(buff), sizeof(buff));
    std::cout << buff;
    file.read((char *)&(buff), sizeof(buff));
    std::cout << buff;
    file.read((char *)&(buff), sizeof(buff));
    std::cout << buff;
    file.read((char *)&(buff), sizeof(buff));
    std::cout << buff << std::endl;;
  }
}


