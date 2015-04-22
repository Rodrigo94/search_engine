/*
 * Text Parser:
 * Input: A clean text
 * Output: A dictionary that maps the frequency of each word in the text
 *
 * Each word in the text is converted to lower case.
 *
 * This parser ignores whitespaces, punctuations and special characters.
 */


#ifndef TEXT_PARSER_H_
#define TEXT_PARSER_H_

#include <algorithm>
#include <wctype.h>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>


std::map<std::string, int> parse_text(std::string text);

#endif /* TEXT_PARSER_H_ */
