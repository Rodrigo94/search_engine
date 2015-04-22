#include "index_builder.h"


bool is_useless_char(unsigned int i){
  //return std::isspace(i) | std::iswblank(i) | std::isdigit(i) | std::ispunct(i) | std::iscntrl(i) ;
  return !std::iswalpha(i);
}

std::map<std::string, int> parse_text(std::string text) {

  std::map<std::string, int> word_frequency_table;

  std::istringstream iss(text);
  std::string line;


  while(iss >> line){
    //std::cout << line << std::endl;
    line.erase(std::remove_if(line.begin(), line.end(),  std::ptr_fun(is_useless_char) ), line.end());
    //std::cout << line << "_" << iss.tellg() << std::endl;
    //if(word_frequency_table)
  }

  //std::cout << char(-62) << std::endl;
  return word_frequency_table;
}
