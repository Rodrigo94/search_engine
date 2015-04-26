
#include "extern_ord.h"

// RunsSize is a list of Runs offsets inside the tuples file
// block_size is the amount of data we will read every time
// runsFile is the name of the file that we stored the runs
ExternalSorter::ExternalSorter(const LintVec& RunsSize, uint block_size, std::string runsFile){
  RunsSize_ = RunsSize; // Runs offsets
  block_size_ = block_size; // Amount of data read
  runsFile_ = runsFile; // File name
  runs_file.open(runsFile.c_str(), std::ifstream::in | std::ifstream::binary);
}

ExternalSorter::~ExternalSorter(){

}

// Starting function, read every run with relatives offsets equal to 0
void ExternalSorter::ReadAllRuns(){
  runs_file.seekg(0, runs_file.beg);
  int run_number = 0;
  for(auto offset: RunsSize_){
    TupleVector tuple_vec;
    TupleRun tuple_run(tuple_vec, RunsSize_[run_number], block_size_, run_number);
    // Push a vector that will store each run inside the structure 'Runs'
    Runs.push_back(tuple_run);
    // Read the i'th run from the file
    ReadOneRun(run_number, offset, 0);
    run_number++;
  }
}

// Read an arbitrary run from the file
void ExternalSorter::ReadOneRun(uint run_number, Lint offset, uint offset_inside_this_run){
  char* buffer = new char[block_size_];
  // Set the file to the proper position
  runs_file.seekg(offset+offset_inside_this_run);
  runs_file.read(buffer, block_size_);
  // Create a tuple block with the tuples read from the file
  CreateTupleBlock(run_number, buffer);
  Runs[run_number].IncRelativeOffset();
  delete[] buffer;
}

// Create a tuple block with the tuples read from the file
void ExternalSorter::CreateTupleBlock(uint run_number, char* buffer){
  uint amount_read = 0;
  while(amount_read < block_size_){
    uint term_number;
    uint doc_number;
    uint term_frequency;
    uint term_position;

    std::copy(&buffer[amount_read], &buffer[amount_read] + sizeof(uint), reinterpret_cast<char*>(&term_number));
    if(int(term_number) < 0) break; // If we reach the padding bytes

    std::copy(&buffer[amount_read] + sizeof(uint), &buffer[amount_read] + 2*sizeof(uint), reinterpret_cast<char*>(&doc_number));
    std::copy(&buffer[amount_read] + 2*sizeof(uint), &buffer[amount_read] + 3*sizeof(uint), reinterpret_cast<char*>(&term_frequency));
    std::copy(&buffer[amount_read] + 3*sizeof(uint), &buffer[amount_read] + 4*sizeof(uint), reinterpret_cast<char*>(&term_position));

    Tuple tuple(term_number, doc_number, term_frequency, term_position);
    tuple.printTuple();
    Runs[run_number].InsertTuple(tuple);
    amount_read += 4*sizeof(uint);
  }
}

void ExternalSorter::Sort(){
  for(auto it: Runs){
    Q.push(it);
  }
  while(!PopSmaller()){

  }
}

void ExternalSorter::Insert(const Tuple& tuple){
 // Q.push(tuple);
}

bool ExternalSorter::PopSmaller(){
  TupleRun min_tuple_run = Q.top();
  bool is_a_padding_byte = min_tuple_run.RemoveFirst();
  if(min_tuple_run.Empty()){
    // Read more b bytes from this run, increment the offset of the tuple run;
    ReadOneRun(min_tuple_run.getRunNumber(), min_tuple_run.getRunOffset(), min_tuple_run.getRunRelativeOffset());
  }
  return is_a_padding_byte;
}

void ExternalSorter::PrintRuns(){
  for(auto it: Runs){
    //for(auto jt: it){
    //   //jt.printTuple();
    //}
  }
}
