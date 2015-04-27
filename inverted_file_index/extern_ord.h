
#ifndef EXTERN_ORD_H_
#define EXTERN_ORD_H_

#include "index_builder.h"
#include "tuple.h"
#include <iostream>
#include <memory>
#include <queue>


class ExternalSorter{
private:
  std::priority_queue<TupleRun, std::vector<TupleRun>,  TupleRun::compare_vector> Q;
  std::vector<Lint> RunsOffsets; // Stores offsets in the file for each run
  std::vector<TupleRun> Runs; // This store a vector of vector of tuples (one vector for each run)
  std::string runsDict_;
  std::string runsFile_;
  std::vector<std::shared_ptr<std::ifstream> > runs_file_set;
  std::ofstream out_file;
  uint block_size_;
  uint out_block_size;
  Lint runs_file_size;
  IntVec buffer_output;
public:
  ExternalSorter(const LintVec& RunsSize, uint block_size, std::string runsFile);
  ~ExternalSorter();
  void ReadAllRuns();
  void ReadOneRun(uint run_number);
  void CreateTupleBlock(uint run_number);
  void Sort();
  void PushTuple(Tuple tuple);
  void DumpTupleBuffer();
};

#endif /* EXTERN_ORD_H_ */
