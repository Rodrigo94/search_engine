
#include "extern_ord.h"

// RunsSize is a list of Runs offsets inside the tuples file
// block_size is the amount of data we will read every time
// runsFile is the name of the file that we stored the runs
ExternalSorter::ExternalSorter(const LintVec& RunsSize, uint block_size, std::string runsFile){
  RunsOffsets = RunsSize; // Runs offsets
  block_size_ = block_size; // Amount of data read
  out_block_size = MEMORY - block_size*RunsSize.size();
  runsFile_ = runsFile; // File name
  buffer_output.clear();
  std::ifstream runs_file;
  runs_file.open(runsFile_.c_str(), std::ifstream::in | std::ifstream::ate | std::ifstream::binary);
  runs_file_size = runs_file.tellg();
  runs_file.close();
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

    std::ifstream run(runsFile_.c_str(), std::ifstream::in | std::ifstream::binary);
    //run.seekg(offset ,run.beg);

    runs_file_set.push_back(std::make_shared<std::ifstream>(runsFile_.c_str()));
    runs_file_set[run_number]->seekg(offset, runs_file_set[run_number]->beg);

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
  Runs[run_number].ReadMoreData(*runs_file_set[run_number]);
}

void ExternalSorter::Sort(){
  out_file.open("extern_sorting_out", std::ifstream::out | std::ifstream::binary);
  int i;
  // While there is elements to pop out of the heap:
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

    // We remove it from the run (writing it to the temporary vector
    PushTuple(min_tuple_run.First());
    min_tuple_run.Pop();

    // If the run is empty, we try to read more data
    if(min_tuple_run.Empty()){
      std::cout << ++i << " runs esvaziadas!" << std::endl;
      // If it has more to read, we read
      if(min_tuple_run.HasMoreToRead()){
        min_tuple_run.ReadMoreData(*runs_file_set[min_tuple_run.getRunNumber()]);
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
  if(buffer_output.size() > 0){
    DumpTupleBuffer();
  }
}

void ExternalSorter::PushTuple(Tuple tuple){
  uint term_number = tuple.TermNumber();
  uint doc_number = tuple.DocumentNumber();
  uint term_frequency = tuple.Frequency();
  uint term_position = tuple.Position();
  buffer_output.push_back(term_number);
  buffer_output.push_back(doc_number);
  buffer_output.push_back(term_frequency);
  buffer_output.push_back(term_position);
  if(buffer_output.size()*4 >= out_block_size){
    DumpTupleBuffer();
    buffer_output.clear();
  }
}

void ExternalSorter::DumpTupleBuffer(){
  uint* buffer = &buffer_output[0];
  out_file.write((char*)buffer, buffer_output.size()*4);
  std::cout << "Dei um dump" << std::endl;
}
