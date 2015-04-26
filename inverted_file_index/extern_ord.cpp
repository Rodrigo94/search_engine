
#include "extern_ord.h"

// RunsSize is a list of Runs offsets inside the tuples file
// block_size is the amount of data we will read every time
// runsFile is the name of the file that we stored the runs
ExternalSorter::ExternalSorter(const LintVec& RunsSize, uint block_size, std::string runsFile){
  RunsOffsets = RunsSize; // Runs offsets
  block_size_ = block_size; // Amount of data read
  runsFile_ = runsFile; // File name
  runs_file.open(runsFile.c_str(), std::ifstream::in | std::ifstream::ate | std::ifstream::binary);
  runs_file_size = runs_file.tellg();
  runs_file.seekg(0, runs_file.beg);
  ReadAllRuns();
  for(uint i = 0; i < Runs.size(); i++){
    Q.push(Runs[i]);
    Runs[i].~TupleRun();
  }
  Runs.clear();
  std::vector<TupleRun>(Runs).swap(Runs);
}

ExternalSorter::~ExternalSorter(){

}

// Starting function, read every run with relatives offsets equal to 0
void ExternalSorter::ReadAllRuns(){
  uint run_number = 0;
  for(auto offset: RunsOffsets){
    // Read the i'th run from the file
    TupleVector tuple_vec;
    Lint next_run_offset;
    if(run_number < RunsOffsets.size() - 1 ){
      next_run_offset = RunsOffsets[run_number + 1];
    } else {
      next_run_offset = runs_file_size;
    }
    TupleRun tuple_run(tuple_vec, offset, next_run_offset, block_size_, run_number);
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
  int i=0;
  out_file.open("extern_sorting_out", std::ifstream::out | std::ifstream::binary);
  while (!Q.empty()){

    // Get the smaller element of the heap
    TupleRun min_tuple_run = Q.top();
    // Remove it from the heap
    Q.pop();
    // I fill the padding bytes with 1 billion.. Since it is 'impossible' to reach that
    // number of terms
    if(min_tuple_run.First().TermNumber() >= uint(1<<30)){
      //std::cout << "Brekou" << std::endl;
      break;
    }

    // We remove it from the run (writing it to the outfile)
    min_tuple_run.PopWrite(out_file);
    Tuple tu = min_tuple_run.Last();


    // If the run is empty, we try to read more data
    if(min_tuple_run.Empty()){
      std::cout << "Run:" << min_tuple_run.getRunNumber() << " lida" << std::endl;
      // If it has more to read, we read
      if(min_tuple_run.HasMoreToRead()){
        std::cout << i << " Runs enviadas" << std::endl;
        i++;
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

