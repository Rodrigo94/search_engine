/*
Trabalho Prático 1 - Arquivo Invertido
*/
#include "index_builder.h"
#include "extern_ord.h"
#include "query_processor.h"

int main(int argc, char* argv[]) {

  // This is the path of index file and collection, change it according to your need
  std::string indexFile =  "index.txt";
  std::string inputDir = "/home/rodrigo/Downloads/irCollection";


  // Remove comments in order to read a collection:
  // You can measeure the time of the next two lines
  Index index(indexFile, inputDir);
  index.index_documents();


  // I decided to put the merging outside the indexing function
  // Use the 'temp' file previously generated to do it
  // Remember to remove the 'temp' file after this step
  // I didn't remove it by myself because it is safer to keep it alive
  ExternalSorter extern_sorter("temp");


  // Set the path to the exern_sorter output
  // The name of the index is by default 'extern_sorting_out'
  std::string path = "/home/rodrigo/Devel/search_engine/";
  // Construct the Query Processor using the path+'extern_sorting_output'
  QueryProcessor Q(path+"extern_sorting_out");
  // Given the path to the vocabulary dumped by the indexing, process it
  Q.process_vocabulary(path+"vocabulary");
  // Now you can process queries by yourself! Press Ctrl+C to finish
  while(1){
    std::string query;
    getline(std::cin, query);
    if(query.compare("x") == 0)
      break;
    Q.process_query(query);
  }

  return 0;
}

