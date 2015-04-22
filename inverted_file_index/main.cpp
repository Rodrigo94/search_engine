/*
Trabalho Prático 1 - Arquivo Invertido
*/
#include <iostream>
#include <fstream>
#include <string>

#include "clean_text.h"
#include "CollectionReader.h"
#include "index_builder.h"

using namespace RICPNS;

#define MEMORY (40*1<<8)
#define w sizeof(tuple_record)
#define INITIAL_RUN_SIZE (50*1<<10)
#define NUM_OF_DOCUMENTS 1


int main(int argc, char* argv[]) {

  std::map<std::string, int> vocabulary;

  std::ofstream temp ("temp", std::ofstream::binary);

  unsigned int k = (MEMORY - vocabulary.size()) / w;
  unsigned int b = INITIAL_RUN_SIZE;
  unsigned int R = 0;

  std::vector<struct tuple_record> tuples_vector;

  // Read documents from the collection:

  // Set up the files:
  std::string inputDirectory("/home/rodrigo/Devel/search_engine/toyExample");
  std::string indexFileName("indexToCompressedColection.txt");

  // Set up the reader
  CollectionReader * reader = new CollectionReader(inputDirectory,
                             indexFileName);

  Document doc;
  //doc.clear();
  int i = 0;
  reader->getNextDocument(doc);
  // Reads the document:
  while(reader->getNextDocument(doc)) {

    std::cout << doc.getURL() << std::endl;
    // Calls the google gumbo-parsers

    std::string parser_result = clean_html(doc.getText());
    doc.clear();

    // Now we are supposed to run the indexer:
    indexer(parser_result, vocabulary, tuples_vector, i);

    k = (MEMORY - vocabulary.size()) / w;
    // If there is so much tuples, dump them into the temp file:
    if ( tuples_vector.size() == k ) {
      tuples_vector.clear();
      R++;
      if ( b*(R + 1) > MEMORY ) {
        b = b / 2;
      }
    }

    i++;
  }

  if(temp.is_open()){
    temp.close();
  }

  delete reader;

  return 0;
}

