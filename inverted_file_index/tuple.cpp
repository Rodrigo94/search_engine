#include "tuple.h"



Tuple::Tuple(uint term_number, uint document_number, uint frequency, uint position){
  term_number_ = term_number;
  document_number_ = document_number;
  frequency_ = frequency;
  position_ = position;
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

void Tuple::printTuple(){
  std::cout << "<" << TermNumber() << ", " << DocumentNumber() << ", " << Frequency() << ", " << Position() << ">" << std::endl;
}

void Tuple::writeTuple(std::ofstream& temp){
  temp.write((char*)&term_number_, sizeof(uint));
  temp.write((char*)&document_number_, sizeof(uint));
  temp.write((char*)&frequency_, sizeof(uint));
  temp.write((char*)&position_, sizeof(uint));
}

bool Tuple::sameDocument(Tuple i){
  return this->term_number_ == i.term_number_ && this->document_number_ == i.document_number_;
}

/*
 * ************************************************************************************************
 * ** Tuple run implementation: *******************************************************************
 * ************************************************************************************************
 */

TupleRun::TupleRun(std::vector<Tuple>& Run, long long int run_offset, uint block_size, uint run_number){
  this->Run = Run;
  this->run_offset = run_offset;
  this->block_size = block_size;
  this->run_number = run_number;
  run_relative_offset = 0;
}

TupleRun::~TupleRun(){

}

Tuple TupleRun::First(){
  return Run.front();
}

void TupleRun::Pop(){
  Run.front().printTuple();
  Run.erase(Run.begin());
}

bool TupleRun::HasMoreToRead(){

  return 1;
}

void TupleRun::ReadMoreData(std::ifstream& file){
  char* buffer = new char[block_size];
  // Set the file to the proper position
  file.seekg(run_offset+run_relative_offset);
  file.read(buffer, block_size);
  // Create a tuple block with the tuples read from the file
  uint amount_read = 0;
  std::vector<Tuple> tuple_vec;
  while(amount_read < block_size){
    uint term_number;
    uint doc_number;
    uint term_frequency;
    uint term_position;

    std::copy(&buffer[amount_read], &buffer[amount_read] + sizeof(uint), reinterpret_cast<char*>(&term_number));
    if(int(term_number) < 0) break; // If we reach the padding bytes

    std::copy(&buffer[amount_read] + sizeof(uint), &buffer[amount_read] + 2*sizeof(uint), reinterpret_cast<char*>(&doc_number));
    std::copy(&buffer[amount_read] + 2*sizeof(uint), &buffer[amount_read] + 3*sizeof(uint), reinterpret_cast<char*>(&term_frequency));
    std::copy(&buffer[amount_read] + 3*sizeof(uint), &buffer[amount_read] + 4*sizeof(uint), reinterpret_cast<char*>(&term_position));

    Tuple tuple(term_number, doc_number, term_frequency, term_position);
    //tuple.printTuple();
    tuple_vec.push_back(tuple);

    amount_read += 4*sizeof(uint);
  }
  delete[] buffer;
  tuple_vec.swap(Run);
  tuple_vec.clear();
  std::vector<Tuple>(tuple_vec).swap(tuple_vec);
}

void TupleRun::IncRelativeOffset(){
  run_relative_offset += block_size;
}

bool TupleRun::Empty(){
  return Run.empty();
}


void TupleRun::InsertTupleVec(std::vector<Tuple>& tuple_vec){
  tuple_vec.swap(Run);
}

 long long int TupleRun::getRunOffset(){
   return run_offset;
}

uint TupleRun::getRunRelativeOffset(){
  return run_relative_offset;
}

uint TupleRun::getRunNumber(){
  return run_number;
}

void TupleRun::DeleteRun(){
  Run.clear();
  std::vector<Tuple>(Run).swap(Run);
}
