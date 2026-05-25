#include <iostream>
#include "aho-corasick.h"

int main() {
    std::vector<std::string> patterns = {"he", "she", "his", "hers", "a", "aa", "aaa", "aaaa", "i"};
    Trie trie(patterns);

    trie.build_failed_links();
    vector<std::pair<int, int> > ans = trie.find_occurences("ushersaaaahisxyzihe");
    for (auto z : ans) {
        std::cout << z.first << " " << z.second << "\n";
    }
}