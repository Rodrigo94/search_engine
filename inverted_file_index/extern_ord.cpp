
#include "extern_ord.h"

// RunsSize is a list of Runs offsets inside the tuples file
// block_size is the amount of data we will read every time
// runsFile is the name of the file that we stored the runs
ExternalSorter::ExternalSorter(){
  // Get the size of runs file
  std::ifstream runs_file;
  runs_file.open("temp", std::ifstream::in | std::ifstream::ate | std::ifstream::binary);
  runs_file_size = runs_file.tellg();
  runs_file.close();

  num_of_runs = runs_file_size / MEMORY;

  // Fill each run with data from this file
  ReadAllRuns();
  for(uint i = 0; i < Runs.size(); i++){
    Q.push(Runs[i].First());
    Runs[i].Pop();
  }

  amount_dumped = 0;
}

ExternalSorter::~ExternalSorter(){

}

// Starting function, read every run with relatives offsets equal to 0
void ExternalSorter::ReadAllRuns(){
  uint run_number = 0;
  for(uint i=0; i<num_of_runs; i++){
    // Read the i'th run from the file
    TupleVector tuple_vec;
    TupleRun tuple_run(tuple_vec, run_number);
    // Push a vector that will store each run inside the structure 'Runs'
    Runs.push_back(tuple_run);

    runs_file_set.push_back(std::make_shared<std::ifstream>("temp"));
    runs_file_set[run_number]->seekg(i*MEMORY, runs_file_set[run_number]->beg);

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
  // While there is elements to pop out of the heap:
  std::cout << "Extern Sorting:" << std::endl;
  while (!Q.empty()){

    // Get the smaller element of the heap
    Tuple min_tuple = Q.top();
    uint min_tuple_run_number = min_tuple.RunNumber();
    // Remove it from the heap
    Q.pop();
    // I fill the padding bytes with 1 billion.. Since it is 'impossible' to reach that
    // number of terms
    if(min_tuple.TermNumber() >= uint(1<<30)){
      //std::cout << "Brekou" << std::endl;
      break;
    }

    // We remove it from the run (writing it to the temporary vector
    PushTuple(min_tuple);
    Runs[min_tuple_run_number].Pop();

    // If the run is empty, we try to read more data
    if(Runs[min_tuple_run_number].Empty()){
      // If it has more to read, we read
      if(Runs[min_tuple_run_number].HasMoreToRead()){
        Runs[min_tuple_run_number].ReadMoreData(*runs_file_set[min_tuple_run_number]);
        Runs[min_tuple_run_number].IncRelativeOffset();
        Q.push(Runs[min_tuple_run_number].First());
      } else {
        // We are done with this run
      }
    } else {
      // We give the heap the same run without the first element.
      // So the heap can merge again.
      Q.push(Runs[min_tuple_run_number].First());
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
  uint run_number = tuple.RunNumber();
  buffer_output.push_back(term_number);
  buffer_output.push_back(doc_number);
  buffer_output.push_back(term_frequency);
  buffer_output.push_back(term_position);
  buffer_output.push_back(run_number);
  if(buffer_output.size()*sizeof(uint) >= MEMORY){
    DumpTupleBuffer();
    buffer_output.clear();
  }
}

void ExternalSorter::DumpTupleBuffer(){
  uint* buffer = &buffer_output[0];
  out_file.write((char*)buffer, MEMORY);
  amount_dumped += MEMORY;
  std::cout << "Total dumped: " << 100*amount_dumped/runs_file_size << "\%" << std::endl;
}
