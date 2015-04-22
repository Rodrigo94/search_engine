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
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>


std::map<std::string, int> parse_text(std::string text);

#endif /* INDEX_BUILDER_H_ */
