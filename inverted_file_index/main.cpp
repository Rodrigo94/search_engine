/*
Trabalho Prático 1 - Arquivo Invertido
*/
#include "index_builder.h"
#include "extern_ord.h"


int main(int argc, char* argv[]) {

  std::string indexFile =  "index.txt";
  std::string inputDir = "/home/rodrigo/Downloads/irCollection";
  //std::string indexFile =  "indexToCompressedColection.txt";
  //std::string inputDir = "/home/rodrigo/Devel/search_engine/toyExample";

  Index index(MEMORY/w, INITIAL_RUN_SIZE, indexFile, inputDir);

  index.index_documents();

  for(auto it: index.getRunsOffsetsVector()){
    std::cout << it << " ";
  }
  std::cout << std::endl << index.getBlockSize() << ", " << std::endl;

  LintVec RunsOffsets =  index.getRunsOffsetsVector();
  uint block_size = index.getBlockSize();

  ExternalSorter extern_sorter(RunsOffsets, block_size, "temp");
  extern_sorter.Sort();

  std::cout << "Deu" << std::endl;
  return 0;
}

