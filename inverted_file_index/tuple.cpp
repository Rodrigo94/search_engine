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

TupleRun::TupleRun(std::vector<Tuple>& Run, long long int run_offset, uint block_size, uint run_number){
  this->Run = Run;
  this->run_offset = run_offset;
  this->block_size = block_size;
  this->run_number = run_number;
  run_relative_offset = 0;
}

TupleRun::~TupleRun(){

}

bool TupleRun::RemoveFirst(){
  Run.front().printTuple();
  bool is_a_padding_byte = (int(Run.front().TermNumber()) < 0);
  Run.erase(Run.begin());
  return is_a_padding_byte;
}

void TupleRun::IncRelativeOffset(){
  run_relative_offset += block_size;
}

bool TupleRun::Empty(){
  return Run.empty();
}


void TupleRun::InsertTuple(Tuple& tuple){
  Run.push_back(tuple);
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
