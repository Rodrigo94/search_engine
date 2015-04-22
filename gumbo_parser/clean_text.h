

#ifndef CLEAN_TEXT_H_
#define CLEAN_TEXT_H_

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include "gumbo.h"


static std::string cleantext(GumboNode* node);

std::string clean_html(std::string contents);


#endif /* CLEAN_TEXT_H_ */
