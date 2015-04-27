/*
Trabalho Prático 1 - Arquivo Invertido
*/
#include "index_builder.h"
#include "extern_ord.h"

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
  Index index(MEMORY/w, RUN_BLOCK_SIZE, indexFile, inputDir);
  index.index_documents();
  std::cout << "Indexing terms stops:" << std::endl;
  display_time();

  index.dump_vocabulary();

  std::cout << "Extern sorting starts:" << std::endl;
  display_time();
  ExternalSorter extern_sorter;
  extern_sorter.Sort();
  std::cout << "Extern sorting stops:" << std::endl;
  display_time();

  /*std::ifstream file("extern_sorting_out", std::ifstream::out | std::ifstream::binary);
  uint SIZE = 50000000;
  char* buffer = new char[SIZE];
    // Set the file to the proper position
    std::cout << "Tento dar o read nesse bloco enorme" << std::endl;
    file.read(buffer, SIZE);
    std::cout << "Deveras consegui" << std::endl;
    // Create a tuple block with the tuples read from the file
    uint amount_read = 0;
    //std::vector<Tuple> tuple_vec;
    std::cout << "Transfiro esse bloco pra tupla" << std::endl;
    uint i=0;
    while(amount_read < SIZE){
      uint term_number;
      uint doc_number;
      uint term_frequency;
      uint term_position;
      uint run;

      std::copy(&buffer[amount_read], &buffer[amount_read] + sizeof(uint), reinterpret_cast<char*>(&term_number));
      if(int(term_number) < 0) break; // If we reach the padding bytes

      std::copy(&buffer[amount_read] + sizeof(uint), &buffer[amount_read] + 2*sizeof(uint), reinterpret_cast<char*>(&doc_number));
      std::copy(&buffer[amount_read] + 2*sizeof(uint), &buffer[amount_read] + 3*sizeof(uint), reinterpret_cast<char*>(&term_frequency));
      std::copy(&buffer[amount_read] + 3*sizeof(uint), &buffer[amount_read] + 4*sizeof(uint), reinterpret_cast<char*>(&term_position));
      std::copy(&buffer[amount_read] + 4*sizeof(uint), &buffer[amount_read] + 5*sizeof(uint), reinterpret_cast<char*>(&run));

      Tuple tuple(term_number, doc_number, term_frequency, term_position, run);

      //if(doc_number == 12)
      tuple.printTuple();
      //tuple_vec.push_back(tuple);

      i++;
      amount_read += w;
    }
    std::cout << "Transferi.. e bom deletar o buffer" << std::endl;
    delete[] buffer;
    std::cout << "Deletei" << std::endl;
    //tuple_vec.swap(Run);
    //tuple_vec.clear();
    //std::vector<Tuple>(tuple_vec).swap(tuple_vec);*/

  return 0;
}

