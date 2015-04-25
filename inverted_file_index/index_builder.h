/*
 * Index Builder
 * Input: A clean text
 * Output: A dictionary that maps the frequency of each word in the text
 *
 * Each word in the text is converted to lower case.
 *
 * This parser ignores whitespaces, punctuations and special characters.
 */


#ifndef INDEX_BUILDER_H_
#define INDEX_BUILDER_H_

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "CollectionReader.h"

#define MEMORY (40*(1<<20))
#define w (4*sizeof(int))
#define INITIAL_RUN_SIZE (50*1<<10)
#define NUM_OF_DOCUMENTS 1

// In order to ease the pain
typedef std::map<std::string, unsigned int> Vocabulary;
typedef std::map<uint, uint> IntIntMap;
typedef std::vector<struct tuple_record> TupleVector;
typedef std::vector<uint> IntVec;

using namespace RICPNS;

// Tuple
struct tuple_record {
  uint term_number;
  uint document_number;
  uint frequency;
  uint position;

};

class Index{
private:
  uint k_;
  uint b_;
  uint R_;
  std::ofstream temp;

  CollectionReader * reader;
  Document doc;

  // These two are persistent
  TupleVector tuples_vector;
  Vocabulary vocabulary;

  // These three can be removed from memory over time. They are temporary.
  IntVec index_terms;
  IntVec positions;
  IntIntMap word_frequency;

public:
  Index(uint k, uint b, std::string indexFile, std::string inputDir);
  ~Index();

  void index_documents();
  void index_text(std::string text);
  void push_tuple(uint term_num, uint doc_num);
  void clear_temporaries();
  void dump_tuples();

};




#endif /* INDEX_BUILDER_H_ */
