#include "index_builder.h"

// Function that removes useless characters
bool is_useless_char(uint i){
  //return std::isspace(i) | std::iswblank(i) | std::isdigit(i) | std::ispunct(i) | std::iscntrl(i) ;
  return !std::isalpha(i);
}

bool pair_compare(const std::pair<uint, std::string>& i, const std::pair<uint, std::string>& j) {
  return i.first < j.first;
}


Index::Index(uint k, uint b, std::string indexFile, std::string inputDir){
  k_ = k;
  b_ = b;
  R_ = 0;
  runs_offset = 0;
  temp.open("temp", std::ofstream::out | std::ofstream::binary);
  reader = new CollectionReader(inputDir,indexFile);
  reader->getNextDocument(doc);
}

Index::~Index(){

  tuples_vector.clear();
  TupleVector(tuples_vector).swap(tuples_vector);

  dump_vocabulary();

  vocabulary.clear();
  Vocabulary(vocabulary).swap(vocabulary);

  dump_offsets();

}


void Index::index_documents(){
  int i = 0;
  // Reads the document:
  while(reader->getNextDocument(doc)) {

    // Only for debugging reasons:
    if(i%1000 == 0)
    std::cout << i << ":" << k_ << ":" << doc.getURL() << std::endl;

    // Calls the google's gumbo-parsers
    std::string parser_result = clean_html(doc.getText());
    doc.clear();


    index_text(parser_result);
    parser_result.clear();
    std::string(parser_result).swap(parser_result);

    // Updates the memory available memory to dump the tuples:
    k_ = (MEMORY - vocabulary.size())/w;


    // Create tuples:
    for(uint j = 0; j < index_terms.size(); j++){
      push_tuple(j, i);
    }
    clear_temporaries();
    i++;
  }
  dump_tuples();
  if(temp.is_open()){
    temp.close();
  }

  delete reader;
}


void Index::index_text(std::string text) {

  // We take the vocabulary's length until now (So we can keep the count of the words):
  uint word_count = vocabulary.size();

  // Simple enchantment to iterate over a string word by word
  std::istringstream iss(text);
  std::string word;

  // While there is string to get:
  while(iss >> word){
    // Transform to lowercase
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    // One more enchantment to get the position of this word:
    uint position = uint(iss.tellg()) - word.size();
    // Just in case the tellg returns the null
    if(int(position) < 0){
      position = text.size() - word.size();
    }

    // Another enchantment to remove useless chars (Linear with the word)
    word.erase(std::remove_if(word.begin(), word.end(),  std::ptr_fun(is_useless_char) ), word.end());

    //'word' is the term we were looking for, but it needs to be greater than size==1.
    if(word.size() > 1 && word.size() < 20 && word.find("http") == std::string::npos && word.find("www") == std::string::npos){
      if (vocabulary.count(word) == 0){
        vocabulary[word] = word_count;
        word_count++;
      }
      uint word_num = vocabulary[word];

      index_terms.push_back(word_num);

      // Set the frequency table for this word:
      if ( word_frequency.count(word_num) == 0){
        word_frequency[word_num] = 1;
      } else {
        word_frequency[word_num]++;
      }

      // Set the position:
      positions.push_back(position);
    }
    word.clear();
    std::string(word).swap(word);
  }
}


void Index::push_tuple(uint term_num, uint doc_num){
  Tuple tuple(index_terms[term_num], doc_num,  word_frequency[index_terms[term_num]], positions[term_num]);
  tuples_vector.push_back(tuple);
  if(tuples_vector.size() >= k_){
    uint run_size = dump_tuples();
    runs_offset += run_size;
    clear_temporaries();
    R_++;
    if ( b_*(R_ + 1) > MEMORY ) {
      b_ = b_ / 2;
    }
  }
}

void Index::clear_temporaries(){
  index_terms.clear();
  positions.clear();
  word_frequency.clear();
  IntVec(index_terms).swap(index_terms);
  IntVec(positions).swap(positions);
  IntIntMap(word_frequency).swap(word_frequency);
}

// Dumps the vector of tuples into the outfile
uint Index::dump_tuples(){
  uint run_size = 0;
  RunsOffsetsVector.push_back(runs_offset);
  sort(tuples_vector.begin(), tuples_vector.end(), Tuple::compare());

  // Buffer that will be stored in memory
  uint tuple_size = 4*sizeof(uint);
  char* buffer = new char[tuple_size*tuples_vector.size()];
  //memset(buffer,1<<30,4*sizeof(uint)*tuples_vector.size());

  for ( uint i = 0; i < tuples_vector.size(); i++ ) {
    uint term_num = tuples_vector[i].TermNumber();
    memcpy(buffer + tuple_size*i + 0*sizeof(uint), &term_num,  sizeof(uint));

    uint doc_num = tuples_vector[i].DocumentNumber();
    memcpy(buffer + tuple_size*i + 1*sizeof(uint), &doc_num,  sizeof(uint));

    uint freq = tuples_vector[i].Frequency();
    memcpy(buffer + tuple_size*i + 2*sizeof(uint), &freq,  sizeof(uint));

    uint pos = tuples_vector[i].Position();
    memcpy(buffer + tuple_size*i + 3*sizeof(uint), &pos,  sizeof(uint));

    run_size += tuple_size;
  }
  temp.write(buffer,run_size);
  // Delete this vector and the buffer
  tuples_vector.clear();
  TupleVector(tuples_vector).swap(tuples_vector);
  delete buffer;

  //Adding padding bytes:
  uint padding_bytes_amount = (b_ - (run_size % b_));

  char* padding = new char[padding_bytes_amount];

  memset(padding,1<<30,padding_bytes_amount);
  temp.write(padding,padding_bytes_amount);

  delete padding;

  return run_size + tuple_size*padding_bytes_amount;
}


void Index::dump_vocabulary(){
  std::vector<std::pair<uint, std::string> > dict;
  for(auto it: vocabulary){
    std::pair<uint, std::string> dict_pair(it.second, it.first);
    dict.push_back(dict_pair);
  }
  sort(dict.begin(), dict.end(), pair_compare);
  std::ofstream dict_out_file("vocabulary");
  for(auto it: dict){
    dict_out_file << it.second;
    std::cout << it.second << std::endl;
    dict_out_file << "\n";
  }
  dict_out_file.close();
  dict.clear();
  std::vector<std::pair<uint, std::string> >(dict).swap(dict);
}

void Index::dump_offsets() {
  std::ofstream offset_out_file;
  offset_out_file.open("offsets", std::ifstream::out | std::ifstream::binary);
  for(auto it: RunsOffsetsVector){
    offset_out_file.write((char*)&it, sizeof(long long int));
  }
  offset_out_file.close();
}

std::vector<Lint>& Index::getRunsOffsetsVector(){
  return  RunsOffsetsVector;
}


uint Index::getBlockSize(){
  return b_;
}
