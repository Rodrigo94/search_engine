
#ifndef EXTERN_ORD_H_
#define EXTERN_ORD_H_

#include "index_builder.h"
#include "tuple.h"
#include <iostream>
#include <memory>
#include <queue>


class ExternalSorter{
private:
  std::priority_queue<Tuple, std::vector<Tuple>,  Tuple::inverse_compare> Q;
  std::vector<TupleRun> Runs; // This store a vector of vector of tuples (one vector for each run)
  std::vector<std::shared_ptr<std::ifstream> > runs_file_set;
  std::ofstream out_file;
  IntVec buffer_output;
  uint num_of_runs;
  Lint runs_file_size; // File size from where we are gonna read our runs
  Lint amount_dumped; //Keeps track of the amount of data dumped to the outbug. A debug tool.
  void Sort();
  void PushTuple(Tuple tuple);
  void DumpTupleBuffer();
public:
  ExternalSorter(std::string runs_file_name);
  ~ExternalSorter();
};

#endif /* EXTERN_ORD_H_ */
