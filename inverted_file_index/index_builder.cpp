#include "index_builder.h"

// Function that removes useless characters
bool is_useless_char(uint i){
  return !std::isalnum(i);
}

// In order to order the vocabulary according to term number
bool pair_compare(const std::pair<uint, std::string>& i, const std::pair<uint, std::string>& j) {
  return i.first < j.first;
}


Index::Index(std::string indexFile, std::string inputDir){
  temp.open("temp", std::ofstream::out | std::ofstream::binary);
  reader = new CollectionReader(inputDir,indexFile);
  reader->getNextDocument(doc);
}

Index::~Index(){

}


void Index::index_documents(){
  std::cout << "Indexing terms starts..." << std::endl;
  int R = 0;
  int doc_amount = 0;
  TupleVector tuples_vector; // Will stored tuples
  IntVec indexed_terms; // Stores indexed terms. After every getNextDocument() it is refreshed (It is the text translated to indexes)
  IntIntMap frequencies; // Map for indexed_term -> frequency in a document, so it is also refreshed for every document
  IntVec positions; // Has the same lenght of indexed_terms, but stores the position of each term
  // Reads the document:
  while(reader->getNextDocument(doc)) {

    // Calls the google's gumbo-parsers
    std::string parser_result = clean_html(doc.getText());
    doc.clear();

    // Get indexed terms for this document
    // Remember that this function has collateral effect on its parameters
    index_text(parser_result, indexed_terms, frequencies, positions);
    parser_result.clear();
    std::string(parser_result).swap(parser_result);

    // Create tuples:
    for(uint i = 0; i < indexed_terms.size(); i++){
      Tuple tuple(indexed_terms[i], doc_amount,  frequencies[indexed_terms[i]], positions[i], R);
      tuples_vector.push_back(tuple);
      if(tuples_vector.size()*TUPLE_SIZE >= MEMORY){
        dump_tuples(tuples_vector);
        R++;
      }
    }
    // Clean up the temporaries structures
    indexed_terms.clear();
    frequencies.clear();
    positions.clear();
    IntVec(indexed_terms).swap(indexed_terms);
    IntIntMap(frequencies).swap(frequencies);
    IntVec(positions).swap(positions);
    doc_amount++;
  }
  // Just in case we still got more tuples (almost unlike)
  dump_tuples(tuples_vector);
  if(temp.is_open()){
    temp.close();
  }
  dump_vocabulary();
  delete reader;
  std::cout << "Indexing terms stops!" << std::endl;
}

// Transform the text into a vector of indexed terms
// The vector of indexed terms, the map of frequencies for each term and the positions of each term
// must be passed as parameters that will suffer collateral effects.
void Index::index_text(std::string& text, IntVec& indexed_terms, IntIntMap& frequencies, IntVec& positions) {

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

    //'word' is the term we were looking for, but it needs to be greater than size==1 and smasller than size==20
    if(word.size() > 1 && word.size() < 20){
      if (vocabulary.count(word) == 0){
        vocabulary[word] = word_count;
        word_count++;
      }
      uint word_num = vocabulary[word];
      indexed_terms.push_back(word_num);
      // Set the frequency table for this word:
      if ( frequencies.count(word_num) == 0){
        frequencies[word_num] = 1;
      } else {
        frequencies[word_num]++;
      }
      // Set the position:
      positions.push_back(position);
    }
    word.clear();
    std::string(word).swap(word);
  }
}

// Dumps the vector of tuples into the outfile
void Index::dump_tuples(TupleVector& tuples_vector){
  sort(tuples_vector.begin(), tuples_vector.end(), Tuple::compare());

  // Buffer that will be stored in memory
  char* buffer = new char[MEMORY];

  for ( uint i = 0; i < tuples_vector.size(); i++ ) {
    uint term_num = tuples_vector[i].TermNumber();
    memcpy(buffer + TUPLE_SIZE*i + 0*sizeof(uint), &term_num,  sizeof(uint));

    uint doc_num = tuples_vector[i].DocumentNumber();
    memcpy(buffer + TUPLE_SIZE*i + 1*sizeof(uint), &doc_num,  sizeof(uint));

    uint freq = tuples_vector[i].Frequency();
    memcpy(buffer + TUPLE_SIZE*i + 2*sizeof(uint), &freq,  sizeof(uint));

    uint pos = tuples_vector[i].Position();
    memcpy(buffer + TUPLE_SIZE*i + 3*sizeof(uint), &pos,  sizeof(uint));

    uint run = tuples_vector[i].RunNumber();
    memcpy(buffer + TUPLE_SIZE*i + 4*sizeof(uint), &run,  sizeof(uint));

  }
  temp.write(buffer,MEMORY);
  // Delete this vector and the buffer
  tuples_vector.clear();
  TupleVector(tuples_vector).swap(tuples_vector);
  delete buffer;
}

// Dumps the vocabulary for this index
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
    dict_out_file << "\n";
  }
  dict_out_file.close();
  dict.clear();
  std::vector<std::pair<uint, std::string> >(dict).swap(dict);
  vocabulary.clear();
  Vocabulary(vocabulary).swap(vocabulary);
}

