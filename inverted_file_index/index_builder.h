/*
 * Index Builder
 * This class must be able to read the collection given and create a index for it
 * The memory cap is 40*(1<<20) - 40*(1<<20)%RUN_BLOCK_SIZE where RUN_BLOCK_SIZE is 50kbytes
 * The - 40*(1<<20)%RUN_BLOCK_SIZE is in order to fit a exact amount of blocks of 50kbytes in MEMORy
 *
 * Usage
 * Construct the index builder with Index( std::string indexFile, std::string inputDir) passing the index file and collection directories as parameters
 * Then call index_documents(). It will spit the file "temp" that will have a set of ordered runs. This file will be the input for the external sorter.
 *
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
#include <string.h>
#include <vector>

#include "CollectionReader.h"
#include "clean_text.h"
#include "tuple.h"

#define MEMORY (40*(1<<20) - 40*(1<<20)%RUN_BLOCK_SIZE)
#define TUPLE_SIZE (5*sizeof(uint))
#define RUN_BLOCK_SIZE (50*(1<<10))

// In order to ease the pain
typedef std::map<std::string, uint> Vocabulary;
typedef std::map<uint, uint> IntIntMap;
typedef std::vector<uint> IntVec;
typedef std::vector<Tuple> TupleVector;
typedef long long int Lint;
typedef std::vector<Lint> LintVec;

using namespace RICPNS;

class Index{
private:
  std::ofstream temp;
  CollectionReader * reader;
  Document doc;
  Vocabulary vocabulary;
  void dump_tuples(TupleVector& tuples_vector);
  void dump_vocabulary();
  void index_text(std::string& text, IntVec& indexed_terms, IntIntMap& frequencies, IntVec& positions);
public:
  Index(std::string indexFile, std::string inputDir);
  ~Index();
  void index_documents();
};

#endif /* INDEX_BUILDER_H_ */
