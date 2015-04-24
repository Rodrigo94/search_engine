#include "index_builder.h"

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

uint indexer(std::string text, Vocabulary& vocabulary, IntVec& index_terms, IntVec& positions, IntIntMap& word_frequency) {

  // This variable keeps the increasing size of the vocabulary:
  uint vocabulary_increase = 0;

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
        vocabulary_increase += sizeof(uint);
        vocabulary_increase += uint(word.size());
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
  }
  iss.clear();
  word.clear();
  return vocabulary_increase;

}

// Dumps the vector of tuples into the outfile
void dump_tuples(TupleVector& tuples_vector, std::ofstream& out, uint padding){
  sort(tuples_vector.begin(), tuples_vector.end(), compare_object);
  out.write((char*)(&((tuples_vector[0]).term_number)), sizeof(int));
  out.write((char*)(&((tuples_vector[0]).document_number)), sizeof(int));
  out.write((char*)(&((tuples_vector[0]).frequency)), sizeof(int));
  out.write((char*)(&((tuples_vector[0]).position)), sizeof(int));
  for ( uint i = 1; i < tuples_vector.size(); i++ ) {
    if(tuples_vector[i-1].term_number == tuples_vector[i].term_number &&
       tuples_vector[i-1].document_number == tuples_vector[i].document_number){
      out.write((char*)(&((tuples_vector[i]).position)), sizeof(int));
    } else {
      out.write((char*)(&((tuples_vector[i]).term_number)), sizeof(int));
      out.write((char*)(&((tuples_vector[i]).document_number)), sizeof(int));
      out.write((char*)(&((tuples_vector[i]).frequency)), sizeof(int));
      out.write((char*)(&((tuples_vector[i]).position)), sizeof(int));
    }
  }
  tuples_vector.clear();
}

