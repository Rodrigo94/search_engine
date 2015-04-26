
#ifndef EXTERN_ORD_H_
#define EXTERN_ORD_H_

#include "index_builder.h"
#include "tuple.h"
#include <queue>


class ExternalSorter{
private:
  std::priority_queue<TupleRun, std::vector<TupleRun>,  TupleRun::compare_vector> Q;
  std::vector<Lint> RunsOffsets; // Stores offsets in the file for each run
  std::vector<TupleRun> Runs; // This store a vector of vector of tuples (one vector for each run)
  std::string runsDict_;
  std::string runsFile_;
  std::ifstream runs_file;
  std::ofstream out_file;
  uint block_size_;
  Lint runs_file_size;
public:
  ExternalSorter(const LintVec& RunsSize, uint block_size, std::string runsFile);
  ~ExternalSorter();
  void ReadAllRuns();
  void ReadOneRun(uint run_number);
  void CreateTupleBlock(uint run_number);
  void Sort();
};

#endif /* EXTERN_ORD_H_ */
