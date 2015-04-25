/*
Trabalho Prático 1 - Arquivo Invertido
*/
#include "index_builder.h"
#include "extern_ord.h"


int main(int argc, char* argv[]) {

  std::string indexFile = "indexToCompressedColection.txt"; //"index.txt";
  std::string inputDir = "/home/rodrigo/Devel/search_engine/toyExample"; //"/home/rodrigo/Downloads/irCollection";

  Index index(MEMORY/w, INITIAL_RUN_SIZE, indexFile, inputDir);

  index.index_documents();

  ExternalSorter extern_sorter(index.getRunsOffsetsVector(), index.getBlockSize(), "temp");
  extern_sorter.ReadAllRuns();
  extern_sorter.PrintRuns();

  return 0;
}

