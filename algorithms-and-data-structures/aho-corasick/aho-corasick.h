#include <memory>
#include <queue>
#include <string>
#include <vector>
using std::shared_ptr;
#define s_ptr shared_ptr

using std::unique_ptr;
#define u_ptr unique_ptr

using std::weak_ptr;
#define w_ptr weak_ptr

using std::queue;
using std::string;
using std::vector;

class TrieNode {
   public:
    u_ptr<TrieNode> children[26];
    TrieNode* fail_link;
    TrieNode* dict_link;
    bool end_of_word;
    int idx_of_word;

    TrieNode();
};

inline TrieNode::TrieNode() {
    for (int i = 0; i < 26; ++i) {
        children[i] = nullptr;
    }

    fail_link = nullptr;
    dict_link = nullptr;
    end_of_word = 0;
    idx_of_word = -1;
}

class Trie {
   private:
    vector<string> all_words;
    u_ptr<TrieNode> Root;

   public:
    Trie() : Root(std::make_unique<TrieNode>()) {}
    Trie(std::vector<std::string> patterns);
    void insert(string word);
    void build_failed_links();
    vector<std::pair<int, int> > find_occurences(string text);
};

inline Trie::Trie(std::vector<std::string> patterns) {
    Root = std::make_unique<TrieNode>();
    for (auto word : patterns) {
        insert(word);
    }
}

inline void Trie::insert(string word) {
    TrieNode* cur = Root.get();
    for (char c : word) {
        if (cur->children[c - 'a'] == nullptr) {
            cur->children[c - 'a'] = std::make_unique<TrieNode>();
        }
        cur = cur->children[c - 'a'].get();
    }

    cur->end_of_word = true;
    cur->idx_of_word = all_words.size();
    all_words.push_back(word);
}

inline void Trie::build_failed_links() {
    Root->fail_link = Root.get();
    queue<TrieNode*> q;
    for (int i = 0; i < 26; ++i) {
        if (Root->children[i]) {
            Root->children[i]->fail_link = Root.get();
            q.push(Root->children[i].get());
        }
    }

    while (!q.empty()) {
        TrieNode* cur = q.front();
        q.pop();
        for (int i = 0; i < 26; ++i) {
            TrieNode* child = cur->children[i].get();
            if (child) {
                q.push(child);

                TrieNode* fail_cur = cur->fail_link;

                while (fail_cur != Root.get() && fail_cur->children[i] == nullptr) {
                    fail_cur = fail_cur->fail_link;
                }

                if (fail_cur->children[i] != nullptr && fail_cur->children[i].get() != child) {
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

inline vector<std::pair<int, int> > Trie::find_occurences(string text) {
    TrieNode* cur = Root.get();
    vector<std::pair<int, int> > answer;
    for (int i = 0; i < text.size(); ++i) {
        char c = text[i];
        while (cur->children[c - 'a'] == nullptr && cur != Root.get()) {
            cur = cur->fail_link;
        }
        if (cur->children[c - 'a']) {
            cur = cur->children[c - 'a'].get();
        }

        if (cur->end_of_word) {
            answer.push_back({i - all_words[cur->idx_of_word].size() + 1, i});
        }

        TrieNode* tempNode = cur->dict_link;

        while (tempNode != nullptr) {
            answer.push_back({i - all_words[tempNode->idx_of_word].size() + 1, i});
            tempNode = tempNode->dict_link;
        }
    }
    return answer;
}