#include "index_builder.h"
#include "clean_text.h"
#include "../test_functions/display_index.h"

// Function that removes useless characters
bool is_useless_char(unsigned int i){
  //return std::isspace(i) | std::iswblank(i) | std::isdigit(i) | std::ispunct(i) | std::iscntrl(i) ;
  return !std::isalpha(i);
}

// Comparision function
struct compare_class {
  bool operator() (struct tuple_record i, struct tuple_record j) {
    if ( i.term_number != j.term_number )
      return (i.term_number < j.term_number);
    if ( i.document_number != j.document_number )
      return (i.document_number < j.document_number);
    if ( i.frequency != j.frequency )
      return (i.frequency < j.frequency);
    return (i.position < j.position);
  }
} compare_object;

Index::Index(uint k, uint b, std::string indexFile, std::string inputDir){
  k_ = k;
  b_ = b;
  R_ = 0;
  temp.open("temp", std::ofstream::out | std::ofstream::binary);
  reader = new CollectionReader(inputDir,indexFile);
  reader->getNextDocument(doc);
}

Index::~Index(){

  tuples_vector.clear();
  TupleVector(tuples_vector).swap(tuples_vector);

  vocabulary.clear();
  Vocabulary(vocabulary).swap(vocabulary);

  dump_tuples();
  if(temp.is_open()){
    temp.close();
  }

  delete reader;
}


void Index::index_documents(){
  int i = 0;
  // Reads the document:
  while(reader->getNextDocument(doc)) {

    // Only for debugging reasons:
    std::cout << i << ":" << k_ << ":" << doc.getURL() << std::endl;

    // Calls the google's gumbo-parsers
    std::string parser_result = clean_html(doc.getText());
    doc.clear();


    // Note that we are looking for the side effects on the previous data structures:
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
    if(position < 0){
      position = text.size() - word.size();
    }

    // Another enchantment to remove useless chars (Linear with the word)
    word.erase(std::remove_if(word.begin(), word.end(),  std::ptr_fun(is_useless_char) ), word.end());

    //'word' is the term we were looking for, but it needs to be greater than size==1.
    if(word.size() > 1){
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
  struct tuple_record tuple;
  tuple.term_number = index_terms[term_num];
  tuple.document_number = doc_num;
  tuple.frequency = word_frequency[index_terms[term_num]];
  tuple.position = positions[term_num];
  tuples_vector.push_back(tuple);
  //std::cout << tuples_vector.size() << std::endl;
  if(tuples_vector.size() >= k_){
    dump_tuples();
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
void Index::dump_tuples(){
  sort(tuples_vector.begin(), tuples_vector.end(), compare_object);
  temp.write((char*)(&((tuples_vector[0]).term_number)), sizeof(int));
  temp.write((char*)(&((tuples_vector[0]).document_number)), sizeof(int));
  temp.write((char*)(&((tuples_vector[0]).frequency)), sizeof(int));
  temp.write((char*)(&((tuples_vector[0]).position)), sizeof(int));
  for ( uint i = 1; i < tuples_vector.size(); i++ ) {
    if(tuples_vector[i-1].term_number == tuples_vector[i].term_number &&
       tuples_vector[i-1].document_number == tuples_vector[i].document_number){
      temp.write((char*)(&((tuples_vector[i]).position)), sizeof(int));
    } else {
      temp.write((char*)(&((tuples_vector[i]).term_number)), sizeof(int));
      temp.write((char*)(&((tuples_vector[i]).document_number)), sizeof(int));
      temp.write((char*)(&((tuples_vector[i]).frequency)), sizeof(int));
      temp.write((char*)(&((tuples_vector[i]).position)), sizeof(int));
    }
  }
  tuples_vector.clear();
  std::vector<struct tuple_record>(tuples_vector).swap(tuples_vector);
}

