/*
Trabalho Prático 1 - Arquivo Invertido
*/
#include "index_builder.h"
#include "extern_ord.h"


int main(int argc, char* argv[]) {

  std::string indexFile =  "index.txt"; //"indexToCompressedColection.txt";
  std::string inputDir = "/home/rodrigo/Downloads/irCollection";  //"/home/rodrigo/Devel/search_engine/toyExample";

  Index index(MEMORY/w, INITIAL_RUN_SIZE, indexFile, inputDir);

  index.index_documents();

  for(auto it: index.getRunsOffsetsVector()){
    std::cout << it;
  }
  std::cout << std::endl << index.getBlockSize() << ", " << std::endl;


  ExternalSorter extern_sorter(index.getRunsOffsetsVector(), index.getBlockSize(), "temp");
  extern_sorter.Sort();
  return 0;
}

