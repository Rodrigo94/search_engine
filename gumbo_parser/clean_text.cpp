// Adaptado de https://github.com/google/gumbo-parser/blob/master/examples/clean_text.cc

#include "clean_text.h"

static std::string cleantext(GumboNode* node) {
  if (node->type == GUMBO_NODE_TEXT) {
    return std::string(node->v.text.text);
  } else if ( node->type == GUMBO_NODE_ELEMENT &&
              node->v.element.tag != GUMBO_TAG_SCRIPT &&
              node->v.element.tag != GUMBO_TAG_STYLE) {
    std::string contents = "";
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
      const std::string text = cleantext((GumboNode*) children->data[i]);
      if (i != 0 && !text.empty()) {
        contents.append(" ");
      }
      contents.append(text);
    }
    return contents;
  } else {
    return "";
  }
}


std::string clean_html(std::string contents) {
  GumboOutput* output = gumbo_parse(contents.c_str());
  std::string cleaned_text = cleantext(output->root);
  gumbo_destroy_output(&kGumboDefaultOptions, output);
  return cleaned_text;
}
