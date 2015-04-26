#ifndef TUPLE_H_
#define TUPLE_H_

#include <iostream>
#include <fstream>
#include <vector>


class Tuple{
private:
  uint term_number_;
  uint document_number_;
  uint frequency_;
  uint position_;
public:
  Tuple(uint term_number, uint document_number, uint frequency, uint position);
  ~Tuple();
  uint TermNumber();
  uint DocumentNumber();
  uint Frequency();
  uint Position();
  void writeTuple(std::ofstream& temp);
  void printTuple();
  bool sameDocument(Tuple i);
  struct compare{
    bool operator()(const Tuple& i, const Tuple& j){
      if ( i.term_number_ != j.term_number_ )
        return (i.term_number_ < j.term_number_);
      if ( i.document_number_ != j.document_number_)
        return (i.document_number_ < j.document_number_);
      return (i.position_ < j.position_);
    }
  };
};


class TupleRun{
private:
  std::vector<Tuple> Run;
  long long int run_offset;
  uint run_relative_offset;
  uint block_size;
  uint run_number;
public:
  TupleRun(std::vector<Tuple>& Run, long long int run_offset, uint block_size, uint run_number);
  ~TupleRun();
  bool RemoveFirst();
  void IncRelativeOffset();
  void InsertTuple(Tuple& tuple);
  bool Empty();
  long long int getRunOffset();
  uint getRunRelativeOffset();
  uint getRunNumber();

  struct compare_vector {
    bool operator() (TupleRun& i, TupleRun& j){
      if ( i.Run.front().TermNumber() != j.Run.front().TermNumber())
        return (i.Run.front().TermNumber() > j.Run.front().TermNumber());
      if ( i.Run.front().DocumentNumber() != j.Run.front().DocumentNumber())
        return (i.Run.front().DocumentNumber() > j.Run.front().DocumentNumber());
      return (i.Run.front().Position() > j.Run.front().Position());
    }
  };
};


#endif /* TUPLE_H_ */
