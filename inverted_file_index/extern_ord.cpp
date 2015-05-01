
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
  for(Lint i=0; i<num_of_runs; i++){
    // Read the i'th run from the file
    TupleVector tuple_vec;
    TupleRun tuple_run(tuple_vec, i);
    // Push a vector that will store each run inside the structure 'Runs'
    Runs.push_back(tuple_run);

    // These two lines set up a the file inside a vector of files
    // Those are responsible for keeping the position read for each run
    // So it turns out that it is possible to read multiple positions for the same file
    // Each position starts at the beggining of each run
    // If the Run becomes empty, we read another block of data without calling any seek
    // So, for the i'th run we just call the 'read' of the file in the i'th position of this vector
    runs_file_set.push_back(std::make_shared<std::ifstream>("temp"));
    runs_file_set[i]->seekg(i*MEMORY, runs_file_set[i]->beg);

    Runs[i].ReadBlockOfData(*runs_file_set[i]);

    // Set up the priority queue with the initial block of each run
    Q.push(Runs[i].First());
  }

  amount_dumped = 0;
}

ExternalSorter::~ExternalSorter(){

}

/*// Starting function, read every run with relatives offsets equal to 0
void ExternalSorter::ReadAllRuns(){
  for(Lint i=0; i<num_of_runs; i++){
    // Read the i'th run from the file
    TupleVector tuple_vec;
    TupleRun tuple_run(tuple_vec, i);
    // Push a vector that will store each run inside the structure 'Runs'
    Runs.push_back(tuple_run);

    runs_file_set.push_back(std::make_shared<std::ifstream>("temp_bk"));
    runs_file_set[i]->seekg(i*MEMORY, runs_file_set[i]->beg);
    ReadOneRun(i);
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
}*/

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

    // We remove it from the run (writing it to the temporary vector
    PushTuple(min_tuple);
    Runs[min_tuple_run_number].Pop();

    // If the run is empty, we try to read more data
    if(Runs[min_tuple_run_number].Empty()){
      // If it has more to read, we read
      if(Runs[min_tuple_run_number].HasMoreToRead()){
        Runs[min_tuple_run_number].ReadBlockOfData(*runs_file_set[min_tuple_run_number]);
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
  buffer_output.push_back(term_number);
  buffer_output.push_back(doc_number);
  buffer_output.push_back(term_frequency);
  buffer_output.push_back(term_position);
  if(buffer_output.size()*sizeof(uint) >= MEMORY){
    std::cout << buffer_output[0] << "," << buffer_output[1] << "," << buffer_output[2] << "," << buffer_output[3] << std::endl;
    DumpTupleBuffer();
    buffer_output.clear();
  }
}

void ExternalSorter::DumpTupleBuffer(){
  uint* buffer = &buffer_output[0];
  out_file.write((char*)buffer, MEMORY);
  amount_dumped += MEMORY;
  std::cout << "Total dumped: " << 125*amount_dumped/runs_file_size << "\%" << std::endl;
}
