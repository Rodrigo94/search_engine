
#include "extern_ord.h"

ExternalSorter::ExternalSorter(const std::vector<uint>& RunsSize, uint block_size, std::string runsFile){
  RunsSize_ = RunsSize;
  block_size_ = block_size;
  runsFile_ = runsFile;
  runs_file.open(runsFile.c_str(), std::ifstream::in | std::ifstream::binary);
}

ExternalSorter::~ExternalSorter(){

}

void ExternalSorter::ReadAllRuns(){
  if(!runs_file.is_open()){
    runs_file.open(runsFile_.c_str(), std::ifstream::in | std::ifstream::binary);
  }
  runs_file.seekg(0, runs_file.beg);
  int i = 0;
  for(auto it: RunsSize_){
    ReadOneRun(i, it, 0);
    i++;
  }
}

void ExternalSorter::ReadOneRun(uint run_number, Lint offset, uint offset_inside_this_run){
  char* buffer = new char[block_size_];
  runs_file.read(buffer, offset + offset_inside_this_run);
  CreateTupleBlock(run_number, buffer);
  delete[] buffer;
}

void ExternalSorter::CreateTupleBlock(uint run_number, char* buffer){
  uint amount_read = 0;
  while(amount_read < block_size_){
    uint term_number;
    uint doc_number;
    uint term_frequency;
    uint term_position;

    std::copy(&buffer[amount_read], &buffer[amount_read] + sizeof(uint), reinterpret_cast<char*>(&term_number));
    if(int(term_number) < 0) break; // If we reach the padding bytes

    std::copy(&buffer[amount_read], &buffer[amount_read + sizeof(uint)] + 2*sizeof(uint), reinterpret_cast<char*>(&doc_number));
    std::copy(&buffer[amount_read], &buffer[amount_read + 2*sizeof(uint)] + 3*sizeof(uint), reinterpret_cast<char*>(&term_frequency));
    std::copy(&buffer[amount_read], &buffer[amount_read + 3*sizeof(uint)] + 3*sizeof(uint), reinterpret_cast<char*>(&term_position));


    Tuple tuple(term_number, doc_number, term_frequency, term_position);
    Runs[run_number].push_back(tuple);
    amount_read += (4*sizeof(uint));
  }
}

void ExternalSorter::Insert(const Tuple& tuple){
 // Q.push(tuple);
}


