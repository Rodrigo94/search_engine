
#ifndef EXTERN_ORD_H_
#define EXTERN_ORD_H_

#include "index_builder.h"
#include "tuple.h"
#include <queue>


class ExternalSorter{
private:
  std::priority_queue<Tuple, TupleVector,  Tuple::compare_rev> Q;
  IntVec RunsSize_;
  std::vector<TupleVector> Runs;
  std::string runsDict_;
  std::string runsFile_;
  std::ifstream runs_file;
  std::ofstream out_file;
  uint block_size_;
  void Insert(const Tuple& T);
  void PopSmaller();
public:
  ExternalSorter(const std::vector<uint>& RunsSize, uint block_size, std::string runsFile);
  ~ExternalSorter();
  void ReadAllRuns();
  void ReadOneRun(uint run_number, Lint offset, uint offset_inside_this_run);
  void CreateTupleBlock(uint run_number, char* buffer);
  void Sort();
};

#endif /* EXTERN_ORD_H_ */
