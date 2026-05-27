#include "aho-corasick.h"

int main() {
  std::vector<std::string> patterns = {
      "cat",      "catch", "dog",  "hot dog", "git",   "github", "the",
      "them",     "there", "here", "where",   "C++",   "CMake",  "VSCodium",
      "standard", "a",     "aa",   "aaa",     "coder", "code"};
  Trie trie(patterns);

  trie.build_failed_links();
  string text =
      "where is my cat? she is looking at github for C++ and CMake inside "
      "VSCodium. standard coders catch dogs and hot dogs here!";
  vector<std::pair<int, int>> ans = trie.find_occurences(text);
  // for (auto z : ans) {
  //     std::cout << z.first << " " << z.second << "\n";
  // }

  trie.make_dot_file("test");
}