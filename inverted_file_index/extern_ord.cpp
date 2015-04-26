
#include "extern_ord.h"

// RunsSize is a list of Runs offsets inside the tuples file
// block_size is the amount of data we will read every time
// runsFile is the name of the file that we stored the runs
ExternalSorter::ExternalSorter(const LintVec& RunsSize, uint block_size, std::string runsFile){
  RunsOffsets = RunsSize; // Runs offsets
  block_size_ = block_size; // Amount of data read
  runsFile_ = runsFile; // File name
  runs_file.open(runsFile.c_str(), std::ifstream::in | std::ifstream::binary);
  ReadAllRuns();
  for(uint i = 0; i < Runs.size(); i++){
    Q.push(Runs[i]);
    Runs[i].DeleteRun();
  }
}

ExternalSorter::~ExternalSorter(){

}

// Starting function, read every run with relatives offsets equal to 0
void ExternalSorter::ReadAllRuns(){
  runs_file.seekg(0, runs_file.beg);
  int run_number = 0;
  for(auto offset: RunsOffsets){
    // Read the i'th run from the file
    TupleVector tuple_vec;
    TupleRun tuple_run(tuple_vec, offset, block_size_, run_number);
    // Push a vector that will store each run inside the structure 'Runs'
    Runs.push_back(tuple_run);
    ReadOneRun(run_number);
    run_number++;
  }
}

// Read an arbitrary run from the file
void ExternalSorter::ReadOneRun(uint run_number){
  // Create a tuple block with the tuples read from the file
  CreateTupleBlock(run_number);
  Runs[run_number].IncRelativeOffset();
}

// Create a tuple block with the tuples read from the file
void ExternalSorter::CreateTupleBlock(uint run_number){
  Runs[run_number].ReadMoreData(runs_file);
}

void ExternalSorter::Sort(){
  int i;
  while (!Q.empty()){

    // Get the smaller element of the heap
    TupleRun min_tuple_run = Q.top();
    // Remove it from the heap
    Q.pop();
    // If it is a -1, we are done (because we are reading padding bytes
    // Remember, we are comparing unsigned integers, so -1 is the largest unsigned int
    if(int(min_tuple_run.First().TermNumber()) == -1){
      std::cout << "Brekou" << std::endl;
      break;
    }

    // We remove it from the run (writing it to the outfile)
    min_tuple_run.Pop();

    // If the run is empty, we try to read more data
    if(min_tuple_run.Empty()){
      // If it has more to read, we read
      if(min_tuple_run.HasMoreToRead()){
        std::cout << "Precisei ir" << std::endl;
        min_tuple_run.ReadMoreData(runs_file);
        min_tuple_run.IncRelativeOffset();
        Q.push(min_tuple_run);
      } else {
        // We are done with this run
      }
    } else {
      // We give the heap the same run without the first element.
      // So the heap can merge again.
      Q.push(min_tuple_run);
    }
  }
}

