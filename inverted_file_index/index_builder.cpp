#include "index_builder.h"

// Function that removes useless characters
bool is_useless_char(unsigned int i){
  //return std::isspace(i) | std::iswblank(i) | std::isdigit(i) | std::ispunct(i) | std::iscntrl(i) ;
  return !std::iswalpha(i);
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

void indexer(std::string text, std::map<std::string, int>& vocabulary, std::vector<struct tuple_record>& tuples_vector, int doc_number) {

  std::cout << "Vamo construir o indice" << std::endl;
  // This will help us to store the frequency of each word:
  std::map<int, int> word_frequency;

  // We take the vocabulary's length until now (So we can keep the count of the words):
  int word_count = vocabulary.size();

  // Simple enchantment to iterate over a string word by word
  std::istringstream iss(text);
  std::string word;

  // While there is string to get:
  while(iss >> word){
    // One more enchantment to get the position of this word:
    int position = int(iss.tellg()) - word.size();
    // Another enchantment to remove useless chars (Linear with the word)
    word.erase(std::remove_if(word.begin(), word.end(),  std::ptr_fun(is_useless_char) ), word.end());
    //'word' is the term we were looking for.
    if (vocabulary.count(word) == 0){
      vocabulary[word] = word_count;
      word_count++;
    }
    int word_num = vocabulary[word];
    // Set the frequency table for this word:
    if ( word_frequency.count(word_num) == 0){
      word_frequency[word_num] = 1;
    } else {
      word_frequency[word_num]++;
    }
    // Now we create the tuple:
    struct tuple_record tuple;
    tuple.term_number = vocabulary[word];
    tuple.document_number = doc_number;
    tuple.frequency = word_frequency[word_num];
    tuple.position = position;
    tuples_vector.push_back(tuple);
  }
  std::cout << "Para o documento " << doc_number << " temos vector tuples de tamanho " << tuples_vector.size() << std::endl;
}

// Dumps the vector of tuples into the outfile
void dump_tuples(std::vector<struct tuple_record>& tuples_vector, std::ofstream& out){
  sort(tuples_vector.begin(), tuples_vector.end(), compare_object);
  for ( std::vector<struct tuple_record>::iterator it = tuples_vector.begin(); it != tuples_vector.end(); it++ ) {
    //out.write(&((*it).document_number), sizeof(int));
    out << (*it).term_number << (*it).document_number << (*it).frequency << (*it).position << std::endl;
  }
  tuples_vector.clear();
}

