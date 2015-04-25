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
  TupleVector Run;
  Lint run_offset;
  uint current_block;
public:
  TupleRun(TupleVector& Run, Lint run_offset);
  ~TupleRun();
  void RemoveFirst();
  void IncCurrentBlock();
  void InsertTuple(Tuple& tuple);

  uint getCurrentBlock();
  Lint getRunOffset();

  struct compare_vector {
    bool operator() (const TupleRun& i, const TupleRun& j){
      if ( i.Run.front().term_number_ != j.Run.front().term_number_ )
        return (i.Run.front().term_number_ > j.Run.front().term_number_);
      if ( i.Run.front().document_number_ != j.Run.front().document_number_)
        return (i.Run.front().document_number_ > j.Run.front().document_number_);
      return (i.Run.front().position_ > j.Run.front().position_);
    }
  };
};


#endif /* TUPLE_H_ */
