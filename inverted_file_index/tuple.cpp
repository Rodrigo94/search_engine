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

void Tuple::writeTuple(std::ofstream& temp){
  temp.write((char*)&term_number_, sizeof(uint));
  temp.write((char*)&document_number_, sizeof(uint));
  temp.write((char*)&frequency_, sizeof(uint));
  temp.write((char*)&position_, sizeof(uint));
}

bool Tuple::sameDocument(Tuple i){
  return this->term_number_ == i.term_number_ && this->document_number_ == i.document_number_;
}
