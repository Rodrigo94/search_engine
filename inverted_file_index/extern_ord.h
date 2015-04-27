
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
  Lint runs_file_size; // File size from where we are gonna read our runs
  IntVec buffer_output;
  uint num_of_runs;
  Lint amount_dumped; //Keeps track of the amount of data dumped to the outbug. A debug tool.
public:
  ExternalSorter();
  ~ExternalSorter();
  void ReadAllRuns();
  void ReadOneRun(uint run_number);
  void CreateTupleBlock(uint run_number);
  void Sort();
  void PushTuple(Tuple tuple);
  void DumpTupleBuffer();
};

#endif /* EXTERN_ORD_H_ */
