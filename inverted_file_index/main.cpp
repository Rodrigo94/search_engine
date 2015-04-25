/*
Trabalho Prático 1 - Arquivo Invertido
*/
#include "index_builder.h"


int main(int argc, char* argv[]) {

  std::string indexFile = "index.txt";
  std::string inputDir = "/home/rodrigo/Downloads/irCollection";

  Index index(MEMORY/w, INITIAL_RUN_SIZE, indexFile, inputDir);

  index.index_documents();
  //std::string inputDirectory("/home/rodrigo/Devel/search_engine/toyExample");
  //std::string indexFileName("indexToCompressedColection.txt");
  //std::string indexFileName("index.txt");
  //std::string inputDirectory("/home/rodrigo/Downloads/irCollection");



  return 0;
}

