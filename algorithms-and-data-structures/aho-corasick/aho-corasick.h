#include <fmt/os.h>

#include <memory>
#include <queue>
#include <string>
#include <vector>

#define s_ptr shared_ptr

using std::unique_ptr;
#define u_ptr unique_ptr

using std::weak_ptr;
#define w_ptr weak_ptr

#define SIZE_OF_ALPHABET 256

using std::queue;
using std::string;
using std::vector;

class TrieNode {
public:
  u_ptr<TrieNode> children[SIZE_OF_ALPHABET];
  TrieNode *fail_link;
  TrieNode *dict_link;
  TrieNode *go[SIZE_OF_ALPHABET];
  bool end_of_word;
  int idx_of_word;
  int id;

  TrieNode();
};

inline TrieNode::TrieNode() {
  for (int i = 0; i < SIZE_OF_ALPHABET; ++i) {
    children[i] = nullptr;
  }

  fail_link = nullptr;
  dict_link = nullptr;
  end_of_word = 0;
  idx_of_word = -1;
  id = -1;
}

class Trie {
private:
  vector<string> all_words;
  u_ptr<TrieNode> Root;
  int number_of_nodes = 1;
  void build_failed_and_dict_links();
  void upgrade_failed_links();

public:
  Trie() : Root(std::make_unique<TrieNode>()) { Root.get()->id = 1; }
  Trie(std::vector<std::string> patterns);
  void insert(string word);

  void build();
  vector<std::pair<int, int>> find_occurences(string text);
  void make_dot_file(string FileName);
};

inline Trie::Trie(std::vector<std::string> patterns) {
  Root = std::make_unique<TrieNode>();
  Root->id = 1;
  for (auto word : patterns) {
    insert(word);
  }
}

inline void Trie::insert(string word) {
  TrieNode *cur = Root.get();
  for (unsigned char c : word) {
    if (cur->children[c] == nullptr) {
      cur->children[c] = std::make_unique<TrieNode>();
    }
    cur = cur->children[c].get();
    if (cur->id == -1) {
      number_of_nodes++;
      cur->id = number_of_nodes;
    }
  }

  cur->end_of_word = true;
  cur->idx_of_word = all_words.size();
  all_words.push_back(word);
}

inline void Trie::build() {
  build_failed_and_dict_links();
  upgrade_failed_links();
}

inline void Trie::build_failed_and_dict_links() {
  Root->fail_link = Root.get();
  queue<TrieNode *> q;
  for (int i = 0; i < SIZE_OF_ALPHABET; ++i) {
    if (Root->children[i]) {
      Root->children[i]->fail_link = Root.get();
      q.push(Root->children[i].get());
    }
  }

  while (!q.empty()) {
    TrieNode *cur = q.front();
    q.pop();
    for (int i = 0; i < SIZE_OF_ALPHABET; ++i) {
      TrieNode *child = cur->children[i].get();
      if (child) {
        q.push(child);

        TrieNode *fail_cur = cur->fail_link;

        while (fail_cur != Root.get() && fail_cur->children[i] == nullptr) {
          fail_cur = fail_cur->fail_link;
        }

        if (fail_cur->children[i] != nullptr &&
            fail_cur->children[i].get() != child) {
          child->fail_link = fail_cur->children[i].get();
        } else {
          child->fail_link = Root.get();
        }

        if (child->fail_link->end_of_word) {
          child->dict_link = child->fail_link;
        } else {
          child->dict_link = child->fail_link->dict_link;
        }
      }
    }
  }
}

inline void Trie::upgrade_failed_links() {
  queue<TrieNode *> q;
  for (int i = 0; i < SIZE_OF_ALPHABET; ++i) {
    if (Root->children[i]) {
      q.push(Root->children[i].get());
      Root->go[i] = Root->children[i].get();
    } else {
      Root->go[i] = Root.get();
    }
  }

  while (!q.empty()) {
    TrieNode *cur = q.front();
    q.pop();

    for (int i = 0; i < SIZE_OF_ALPHABET; ++i) {
      if (cur->children[i]) {
        q.push(cur->children[i].get());
        cur->go[i] = cur->children[i].get();
        continue;
      }
      cur->go[i] = cur->fail_link->go[i];
    }
  }
}

inline vector<std::pair<int, int>> Trie::find_occurences(string text) {
  TrieNode *cur = Root.get();
  vector<std::pair<int, int>> answer;
  for (int i = 0; i < text.size(); ++i) {
    unsigned char c = text[i];
    cur = cur->go[c];

    if (cur->end_of_word) {
      answer.push_back({i - all_words[cur->idx_of_word].size() + 1, i});
    }

    TrieNode *tempNode = cur->dict_link;

    while (tempNode != nullptr) {
      answer.push_back({i - all_words[tempNode->idx_of_word].size() + 1, i});
      tempNode = tempNode->dict_link;
    }
  }
  return answer;
}

inline void Trie::make_dot_file(string FileName) {
  auto out = fmt::output_file(FileName);
  out.print("digraph G {{\n");
  out.print("    layout=dot;\n");
  out.print("    nodesep=0.7;\n");
  out.print("    ranksep=0.8;\n");
  out.print("    node [shape=circle, fontname=\"Helvetica\", width=0.4, "
            "fixedsize=true];\n\n");

  queue<TrieNode *> q;
  q.push(Root.get());

  while (q.size()) {
    TrieNode *cur = q.front();
    q.pop();
    if (cur->end_of_word) {
      out.print("    {} [shape=doublecircle, fillcolor=\"#1fc713ff\", "
                "style=filled, label=\"{}\"];\n",
                cur->id, cur->id);
    } else {
      out.print("    {} [label=\"{}\"];\n", cur->id, cur->id);
    }
    for (int i = 0; i < SIZE_OF_ALPHABET; ++i) {
      if (cur->children[i])
        q.push(cur->children[i].get());
    }
  }

  q.push(Root.get());

  while (q.size()) {
    TrieNode *cur = q.front();
    q.pop();
    for (int i = 0; i < SIZE_OF_ALPHABET; ++i) {
      if (cur->children[i]) {
        q.push(cur->children[i].get());

        out.print("    {} -> {} [color=\"#0055ff\", penwidth=2, fontsize=12, "
                  "label=\" {} \"];\n",
                  cur->id, cur->children[i]->id, char(i));
      }
    }
    if (cur->fail_link && cur->fail_link != Root.get())
      out.print("    {} -> {} "
                "[color=red,style=dotted,constraint=false,dir=forward,penwidth="
                "1.5];\n",
                cur->id, cur->fail_link->id);
    if (cur->dict_link)
      out.print("    {} -> {} "
                "[color=\"#8e10aeff\",style=dotted,constraint=false,dir="
                "forward,penwidth=1.5];\n",
                cur->id, cur->dict_link->id);
  }

  out.print("}}\n");
}
