#include "display_index.h"


void show_file(std::string filename){
  std::ifstream file (filename.c_str(), std::ifstream::in | std::ifstream::binary);
  int off = 1000000000;
  int buff;
  file.seekg(off, file.beg);
  file.seekg(off, file.cur);
  file.seekg(off, file.cur);
  file.seekg(off, file.cur);
  file.seekg(off, file.cur);
  file.seekg(off, file.cur);
  file.seekg(off, file.cur);
  file.seekg(604700000, file.cur);
  for(int i=0; i<10000; i++){
  //while(!file.eof()){
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


