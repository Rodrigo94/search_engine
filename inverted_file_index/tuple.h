/*
 * Tuple
 * This is a simple class that manages tuples.
 *
 */

#ifndef TUPLE_H_
#define TUPLE_H_

#include <iostream>
#include <fstream>
#include <vector>

#define MEM_SIZE (40*(1<<20) - 40*(1<<20)%BLOCK_SIZE)
#define tuple_size (5*sizeof(uint))
#define BLOCK_SIZE (50*(1<<10))

class Tuple{
private:
  uint term_number_;
  uint document_number_;
  uint frequency_;
  uint position_;
  uint run_number_;
public:
  Tuple(uint term_number, uint document_number, uint frequency, uint position, uint run_number);
  ~Tuple();

  uint TermNumber();
  uint DocumentNumber();
  uint Frequency();
  uint Position();
  uint RunNumber();

  void printTuple();

  struct compare{
    bool operator()(const Tuple& i, const Tuple& j){
      if ( i.term_number_ != j.term_number_ )
        return (i.term_number_ < j.term_number_);
      if ( i.document_number_ != j.document_number_)
        return (i.document_number_ < j.document_number_);
      return (i.position_ < j.position_);
    }
  };

  struct inverse_compare{
    bool operator()(const Tuple& i, const Tuple& j){
      if ( i.term_number_ != j.term_number_ )
        return (i.term_number_ > j.term_number_);
      if ( i.document_number_ != j.document_number_)
        return (i.document_number_ > j.document_number_);
      return (i.position_ > j.position_);
    }
  };
};

// This class manages a run of tuples. It is capable of:
// - Store a vector of tuples
// - Store its number
// - Store its offset from the start of the run
// - Read blocks of BLOCK_SIZE from a file;
// - Take the first tuple in a run
class TupleRun{
private:
  std::vector<Tuple> Run;
  uint run_relative_offset;
  uint run_number;
public:
  TupleRun(std::vector<Tuple>& Run, uint run_number);
  ~TupleRun();
  Tuple First(); // Return the first tuple
  void Pop(); // Remove the first tuple
  bool HasMoreToRead(); // If we have already read MEMORY amount of tuples
  bool Empty(); // Is it empty?
  uint getRunNumber(); // Getter
  void ReadBlockOfData(std::ifstream& file); // Reads BLOCK_SIZE of data from 'file'
  void DeleteRun();
};


#endif /* TUPLE_H_ */
