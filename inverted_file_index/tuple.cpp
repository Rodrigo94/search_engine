#include "tuple.h"



Tuple::Tuple(uint term_number, uint document_number, uint frequency, uint position, uint run_number){
  term_number_ = term_number;
  document_number_ = document_number;
  frequency_ = frequency;
  position_ = position;
  run_number_ = run_number;
}

Tuple::~Tuple(){

}

uint Tuple::TermNumber(){
  return term_number_;
}

uint Tuple::DocumentNumber(){
  return document_number_;
}

uint Tuple::Frequency(){
  return frequency_;
}

uint Tuple::Position(){
  return position_;
}

uint Tuple::RunNumber(){
  return run_number_;
}

void Tuple::printTuple(){
  std::cout << "<" << TermNumber() << ", " << DocumentNumber() << ", " << Frequency() << ", " << Position() << ", " << RunNumber() << ">" << std::endl;
}


/*
 * ************************************************************************************************
 * ** Tuple run implementation: *******************************************************************
 * ************************************************************************************************
 */

TupleRun::TupleRun(std::vector<Tuple>& Run, uint run_number){
  this->Run = Run;
  this->run_number = run_number;
  run_relative_offset = 0;
}

TupleRun::~TupleRun(){
  DeleteRun();
}

Tuple TupleRun::First(){
  return Run.front();
}

void TupleRun::Pop(){
  Run.erase(Run.begin());
}

bool TupleRun::HasMoreToRead(){
  if(run_relative_offset  >= MEM_SIZE){
    return false;
  }
  return true;
}

void TupleRun::ReadBlockOfData(std::ifstream& file){
  char* buffer = new char[BLOCK_SIZE];
  // Set the file to the proper position
  file.read(buffer, BLOCK_SIZE);
  this->run_relative_offset += BLOCK_SIZE;
  // Create a tuple block with the tuples read from the file
  uint amount_read = 0;
  std::vector<Tuple> tuple_vec;
  while(amount_read < BLOCK_SIZE){
    uint term_number;
    uint doc_number;
    uint term_frequency;
    uint term_position;
    uint run;

    // Copy a buffer of a tuple size in order to create a tuple
    std::copy(&buffer[amount_read] + 0*sizeof(uint), &buffer[amount_read] + 1*sizeof(uint), reinterpret_cast<char*>(&term_number));
    std::copy(&buffer[amount_read] + 1*sizeof(uint), &buffer[amount_read] + 2*sizeof(uint), reinterpret_cast<char*>(&doc_number));
    std::copy(&buffer[amount_read] + 2*sizeof(uint), &buffer[amount_read] + 3*sizeof(uint), reinterpret_cast<char*>(&term_frequency));
    std::copy(&buffer[amount_read] + 3*sizeof(uint), &buffer[amount_read] + 4*sizeof(uint), reinterpret_cast<char*>(&term_position));
    std::copy(&buffer[amount_read] + 4*sizeof(uint), &buffer[amount_read] + 5*sizeof(uint), reinterpret_cast<char*>(&run));

    Tuple tuple(term_number, doc_number, term_frequency, term_position, run_number);
    tuple_vec.push_back(tuple);

    amount_read += tuple_size;
  }
  delete[] buffer;
  tuple_vec.swap(Run);
  tuple_vec.clear();
  std::vector<Tuple>(tuple_vec).swap(tuple_vec);
}

bool TupleRun::Empty(){
  return Run.empty();
}


void TupleRun::DeleteRun(){
  Run.clear();
  std::vector<Tuple>(Run).swap(Run);
}
