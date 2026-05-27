#include "aho-corasick.h"
#include <iostream>

int main() {
  std::vector<std::string> patterns = {"c", "abc", "bcd"};
  Trie trie(patterns);

  trie.build();
  std::string text = "abcdeabc";
  vector<std::pair<int, int>> ans = trie.find_occurences(text);
  for (auto z : ans) {
      std::cout << z.first << " " << z.second << "\n";
  }

  trie.make_dot_file("test");
}