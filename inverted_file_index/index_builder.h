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

#define MEMORY (40*(1<<20))
#define w (4*sizeof(int))
#define INITIAL_RUN_SIZE (50*1<<10)
#define NUM_OF_DOCUMENTS 1

// In order to ease the pain
typedef std::map<std::string, unsigned int> Vocabulary;
typedef std::map<uint, uint> IntIntMap;
typedef std::vector<struct tuple_record> TupleVector;
typedef std::vector<uint> IntVec;

// Tuple
struct tuple_record {
  uint term_number;
  uint document_number;
  uint frequency;
  uint position;
};


uint indexer(std::string text, Vocabulary& vocabulary, IntVec& index_terms, IntVec& positions, IntIntMap& word_frequency);

// It dumps the vector of tuples into the outfile
void dump_tuples(TupleVector& tuples_vector, std::ofstream& out, uint padding);

#endif /* INDEX_BUILDER_H_ */
