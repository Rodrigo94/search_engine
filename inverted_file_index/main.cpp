/*
Trabalho Prático 1 - Arquivo Invertido
*/
#include "index_builder.h"


int main(int argc, char* argv[]) {

  std::string indexFile = "index.txt"; //"indexToCompressedColection.txt"
  std::string inputDir = "/home/rodrigo/Downloads/irCollection"; //"/home/rodrigo/Devel/search_engine/toyExample"

  Index index(MEMORY/w, INITIAL_RUN_SIZE, indexFile, inputDir);

  index.index_documents();


  return 0;
}

