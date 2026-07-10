#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

struct TrieNode {
    // Dùng unordered_map để tiết kiệm bộ nhớ thay vì mảng tĩnh 26 ký tự
    std::unordered_map<char, std::unique_ptr<TrieNode>> children;
    bool isEndOfWord = false;
};

class Trie {
private:
    std::unique_ptr<TrieNode> root;
    void dfs(TrieNode* node, std::string currentWord, std::vector<std::string>& results, int limit);

public:
    Trie();
    void insert(const std::string& word);
    std::vector<std::string> autocomplete(const std::string& prefix, int limit = 5);
};

#endif