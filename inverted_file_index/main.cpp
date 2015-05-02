/*
Trabalho Prático 1 - Arquivo Invertido
*/
#include "index_builder.h"
#include "extern_ord.h"
#include "query_processor.h"

#include <ctime>

// Display time function. Copied from here:
//
// It is just for debugging goals.
void display_time(){
  time_t current_time;
  struct tm* t;
  char buffer[10];
  time (&current_time);
  t = localtime(&current_time);
  strftime(buffer,10,"%H:%M:%S",t);
  std::cout << buffer << std::endl;
}

int main(int argc, char* argv[]) {

  std::string indexFile =  "index.txt";
  std::string inputDir = "/home/rodrigo/Downloads/irCollection";
  //std::string indexFile =  "indexToCompressedColection.txt";
  //std::string inputDir = "/home/rodrigo/Devel/search_engine/toyExample";

  std::cout << "Indexing terms starts:" << std::endl;
  display_time();
  //Index index(MEMORY/w, RUN_BLOCK_SIZE, indexFile, inputDir);
  //index.index_documents();
  std::cout << "Indexing terms stops:" << std::endl;
  display_time();

  //index.dump_vocabulary();

  std::cout << "Extern sorting starts:" << std::endl;
  display_time();
  //ExternalSorter extern_sorter;
  //extern_sorter.Sort();
  std::cout << "Extern sorting stops:" << std::endl;
  display_time();

  /*std::ifstream file("extern_sorting_out", std::ifstream::in | std::ifstream::binary);
  //uint SIZE = 50000000;
  file.seekg(1631939600, file.beg);
  char* buffer = new char[w-4];
    // Set the file to the proper position
  while(1){
    file.read(buffer, w-4);
    // Create a tuple block with the tuples read from the file

    //std::vector<Tuple> tuple_vec;
    uint term_number;
    uint doc_number;
    uint term_frequency;
    uint term_position;

    // Copy a buffer of a tuple size in order to create a tuple
    std::copy(&buffer[0] + 0*sizeof(uint), &buffer[0] + 1*sizeof(uint), reinterpret_cast<char*>(&term_number));
    std::copy(&buffer[0] + 1*sizeof(uint), &buffer[0] + 2*sizeof(uint), reinterpret_cast<char*>(&doc_number));
    std::copy(&buffer[0] + 2*sizeof(uint), &buffer[0] + 3*sizeof(uint), reinterpret_cast<char*>(&term_frequency));
    std::copy(&buffer[0] + 3*sizeof(uint), &buffer[0] + 4*sizeof(uint), reinterpret_cast<char*>(&term_position));

    //std::cout << term_number << std::endl;

    Tuple tuple(term_number, doc_number, term_frequency, term_position, 0);


    tuple.printTuple();

    if(term_number == 1) exit(1);
    //i++;

  }*/

  //std::string path = "/home/rodrigo/Devel/search_engine/";
  //QueryProcessor Q(path+"extern_sorting_out");
  //Q.process_vocabulary(path+"vocabulary");
  //Q.process_query("nivio ziviani professor");

  /*CollectionReader * reader;
  Document doc;
  reader = new CollectionReader(inputDir,indexFile);
  reader->getNextDocument(doc);
  int i=0;
  while(reader->getNextDocument(doc)) {
    if(i!=715751) {i++;continue;}
    i++;
     // Calls the google's gumbo-parsers
     std::string parser_result = clean_html(doc.getText());
     doc.clear();

     std::cout << parser_result << std::endl;

   }*/

  return 0;
}

