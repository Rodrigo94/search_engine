/*
Trabalho Prático 1 - Arquivo Invertido
*/
#include "clean_text.h"
#include "CollectionReader.h"
#include "index_builder.h"
#include "../test_functions/display_index.h"

using namespace RICPNS;

#define MEMORY (40*1<<20)
#define w sizeof(tuple_record)
#define INITIAL_RUN_SIZE (50*1<<10)
#define NUM_OF_DOCUMENTS 1


int main(int argc, char* argv[]) {

  std::map<std::string, int> vocabulary;

  std::ofstream temp ("temp", std::ofstream::out | std::ofstream::binary);

  unsigned int k = (MEMORY - vocabulary.size()) / w;
  unsigned int b = INITIAL_RUN_SIZE;
  unsigned int R = 0;

  std::vector<struct tuple_record> tuples_vector;

  // Read documents from the collection:

  // Set up the files:
  //std::string inputDirectory("/home/rodrigo/Devel/search_engine/toyExample");
  std::string inputDirectory("/home/rodrigo/Downloads/irCollection");
  //std::string indexFileName("indexToCompressedColection.txt");
  std::string indexFileName("index.txt");

  // Set up the reader
  CollectionReader * reader = new CollectionReader(inputDirectory,
                             indexFileName);

  Document doc;
  int i = 0;
  reader->getNextDocument(doc);
  // Reads the document:
  while(reader->getNextDocument(doc)) {

    // Only for debugging reasons:
    std::cout << i << ":" << doc.getURL() << std::endl;

    // Calls the google's gumbo-parsers
    std::string parser_result = clean_html(doc.getText());
    doc.clear();

    // Now we are supposed to run the indexer:
    indexer(parser_result, vocabulary, tuples_vector, i);

    k = (MEMORY - vocabulary.size()) / w;
    // If there is so much tuples, dump them into the temp file:

    if ( tuples_vector.size() >= k ) {

      dump_tuples(tuples_vector, temp);

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

  //show_file("temp", vocabulary);

  return 0;
}

