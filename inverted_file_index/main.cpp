/*
Trabalho Prático 1 - Arquivo Invertido
*/
#include "clean_text.h"
#include "CollectionReader.h"
#include "index_builder.h"
#include "../test_functions/display_index.h"

using namespace RICPNS;

int main(int argc, char* argv[]) {

  uint k = MEMORY / w;
  uint b = INITIAL_RUN_SIZE;
  uint R = 0;

  Vocabulary vocabulary;
  TupleVector tuples_vector;

  std::ofstream temp ("temp", std::ofstream::out | std::ofstream::binary);
  temp.close();

  // Set up the files:
  //std::string inputDirectory("/home/rodrigo/Devel/search_engine/toyExample");
  //std::string indexFileName("indexToCompressedColection.txt");
  std::string indexFileName("index.txt");
  std::string inputDirectory("/home/rodrigo/Downloads/irCollection");

  // Set up the reader
  CollectionReader * reader = new CollectionReader(inputDirectory,indexFileName);
  Document doc;
  int i = 0;
  reader->getNextDocument(doc);
  // Reads the document:
  while(reader->getNextDocument(doc)) {

    // Only for debugging reasons:
    std::cout << i << ":" << k << ":" << doc.getURL() << std::endl;
    // Calls the google's gumbo-parsers
    std::string parser_result = clean_html(doc.getText());
    doc.clear();

    // Now we are supposed to run the indexer:
    IntVec index_terms;
    IntVec positions;
    IntIntMap word_frequency;

    // Note that we are looking for the side effects on the previous data structures:
    uint vocabulary_increase = indexer(parser_result, vocabulary, index_terms, positions, word_frequency);
    parser_result.clear();

    // Updates the memory available memory to dump the tuples:
    k -= vocabulary_increase / w;

    //std::cout << vocabulary_increase << std::endl;

    // Create tuples:
    for(uint j = 0; j < index_terms.size(); j++){
      struct tuple_record tuple;
      tuple.term_number = index_terms[j];
      tuple.document_number = i;
      tuple.frequency = word_frequency[index_terms[j]];
      tuple.position = positions[j];
      tuples_vector.push_back(tuple);
      std::cout << tuples_vector.size() << std::endl;
      if(tuples_vector.size() >= k){
        dump_tuples(tuples_vector, temp, b);
        R++;
        if ( b*(R + 1) > MEMORY ) {
          b = b / 2;
        }
      }
    }
    index_terms.clear();
    positions.clear();
    word_frequency.clear();
    i++;
  }

  vocabulary.clear();

  dump_tuples(tuples_vector, temp, b);
  if(temp.is_open()){
    temp.close();
  }

  delete reader;
  //show_file("temp");

  return 0;
}

