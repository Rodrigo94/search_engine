/*
Trabalho Prático 1 - Arquivo Invertido
*/
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "CollectionReader.h"

using namespace RICPNS;
using namespace std;

#define MEMORY (40*1<<8)
#define w sizeof(record)
#define INITIAL_RUN_SIZE (50*1<<10)
#define NUM_OF_DOCUMENTS 1

// Suppose I already have a parser
vector<string> parser(int document_id) {
  vector<string> index_terms;
  index_terms.push_back("Oi");
  index_terms.push_back("Bem");
  index_terms.push_back("Tudo");
  index_terms.push_back("Tudo");
  index_terms.push_back("Oi");
  index_terms.push_back("Tudo");
  index_terms.push_back("Bem");
  index_terms.push_back("Bem");
  index_terms.push_back("Bem");
  return index_terms;
}

struct record {
  int term_number;
  int document_number;
  int frequency;
  int position;
};

struct compare_class {
  bool operator() (struct record i, struct record j) {
    if ( i.term_number != j.term_number )
      return (i.term_number < j.term_number);
    if ( i.document_number != j.document_number )
      return (i.document_number < j.document_number);  
    if ( i.frequency != j.frequency )
      return (i.frequency < j.frequency);
    return (i.position < j.position);
  }
} compare_object;

// This function dumps a vector of tuples into the file
void dump_tuples(ofstream out, vector<struct record> tuples_vector) {
  //for ( auto t : tuples_vector ) {
  //
  //}
}


int main(int argc, char* argv[]) {

  map<string, int> vocabulary;
  int word_count = 0;

  ofstream temp ("temp", ofstream::binary);

  unsigned int L = vocabulary.size();
  unsigned int k = (MEMORY - L) / w;
  unsigned int b = INITIAL_RUN_SIZE;
  unsigned int R = 0;

  vector<struct record> tuples_vector;

  for ( int i = 0; i < NUM_OF_DOCUMENTS; i++ ) {
    vector<string> D_i = parser(i);
    map<int, int> word_frequency;
    // First iteration:
    // Maps the vocabulary, word frequencies and sets the memory
    for ( auto it : D_i ) {
      // Set the global word count, vocabulary size and elapsed memory:
      if ( vocabulary.count(it) == 0 ) {
        vocabulary[it] = word_count;
        word_count++;
        L = vocabulary.size();
        k = (MEMORY - L) / w;
      }
      int word_num = vocabulary[it];
      // Set the frequency table for this word:
      if ( word_frequency.count(word_num) == 0 ) {
        word_frequency[word_num] = 1;
      } else {
        word_frequency[word_num]++;
      }
    }
    // Second iteration, fills the tuples vector
    int pos = 0;
    for ( auto it : D_i ) {
      // Push this record to the vector of tuple records:
      struct record tuple;
      tuple.term_number = vocabulary[it];
      tuple.document_number = i;
      tuple.frequency = word_frequency[vocabulary[it]];
      tuple.position = pos;
      tuples_vector.push_back(tuple);

      pos += it.size();

      // Dump to the temp file as soon as the k items limit is reached:
      if ( tuples_vector.size() == k ) {
        sort(tuples_vector.begin(), tuples_vector.end(), compare_object);
        for ( vector<struct record>::iterator jt = tuples_vector.begin(); jt != tuples_vector.end(); jt++ ) {
          //string buffer = "<" + to_string((*jt).term_number) + "," + to_string((*jt).document_number) + "," + to_string((*jt).frequency) + ">\n";
          //temp.write(buffer.c_str(), buffer.size());
        }
        tuples_vector.clear();
        R++;
        if ( b*(R + 1) > MEMORY ) {
          b = b / 2;
        }
      }
    }

  }



  for ( auto it : tuples_vector ) {
    cout << it.term_number << " " << it.position << endl;
  }
  cout << k << endl;

  temp.close();

  cout << "Testing CollectionReader class..." << endl;

  string inputDirectory("/home/rodrigo/Devel/search_engine/toyExample");
  string indexFileName("indexToCompressedColection.txt");

  CollectionReader * reader = new CollectionReader(inputDirectory,
                           indexFileName);
  Document doc;
  doc.clear();
  int i = 0;
  while(reader->getNextDocument(doc)) {
    doc.clear();
    ++i;
  }
  cerr << "Total [" << i << "]" << endl;

  delete reader;

  cout << "Test finished." << endl;

  return 0;
}

