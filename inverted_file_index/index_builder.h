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

// Tuple
struct tuple_record {
  int term_number;
  int document_number;
  int frequency;
  int position;
};

// It changes the vector of tuples
void indexer(std::string text, std::map<std::string, int>& vocabulary, std::vector<struct tuple_record>& tuples_vector, int doc_number);

// It dumps the vector of tuples into the outfile
void dump_tuples(std::vector<struct tuple_record>& tuples_vector, std::ofstream& out);

#endif /* INDEX_BUILDER_H_ */
