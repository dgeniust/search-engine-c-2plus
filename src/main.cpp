#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <cctype>
#include <cmath>
#include <algorithm>
#include "../include/Trie.h"
#include "../include/SpellChecker.h"
#include "../include/LRUCache.h"
#include "../include/InvertedIndex.h"

// Hàm chuẩn hóa: Chuyển thành chữ thường và chỉ giữ lại chữ cái
std::string normalizeString(const std::string& input) {
    std::string result = "";
    for (char c : input) {
        if (std::isalpha(c)) {
            result += std::tolower(c);
        }
    }
    return result;
}

int main() {
    Trie engine;
    InvertedIndex docIndex;
    LRUCache cache(3); // Dung lượng Cache = 3 truy vấn gần nhất
    std::string word;
    int wordCount = 0;

    std::vector<std::string> fullDictionary;

    std::cout << "[System] Dang tai tu dien vao bo nho..." << std::endl;
    auto startLoad = std::chrono::high_resolution_clock::now();

    std::ifstream file("data/dictionary.txt"); 
    if (!file.is_open()) {
        std::cerr << "[Error] Khong the mo file data/dictionary.txt!\n";
        return 1;
    }

    while (std::getline(file, word)) {
        std::string cleanWord = normalizeString(word);
        if (!cleanWord.empty()) {
            engine.insert(cleanWord);
            fullDictionary.push_back(cleanWord);
            wordCount++;
        }
    }
    file.close();

    auto endLoad = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> loadTime = endLoad - startLoad;
    std::cout << "Da tai " << wordCount << " tu. Thoi gian: " << loadTime.count() << " ms\n";
    std::cout << "==================================================\n\n";

    // Nap du lieu cho Inverted Index
    std::cout << "[System] Dang nap Inverted Index...\n";
    docIndex.addDocument("doc1_tech.txt", "apple releases new application for macbook");
    docIndex.addDocument("doc2_food.txt", "how to make apple pie and banana cake");
    docIndex.addDocument("doc3_work.txt", "how to apply for a job and become a boss");
    std::cout << "==================================================\n\n";

    // Vong lap truy van
    std::string rawInput;
    while (true) {
        std::cout << "--> Nhap tu khoa can tim (hoac go 'exit' de thoat): ";
        std::cin >> rawInput;

        if (rawInput == "exit") {
            std::cout << "Tam biet!\n";
            break;
        }

        std::string input = normalizeString(rawInput);
        if (input.empty()) continue;

        auto startQuery = std::chrono::high_resolution_clock::now();
        std::vector<std::string> suggestions;
        bool isFuzzy = false;
        std::vector<std::pair<int, std::string>> fuzzyResults;

        // ==========================================
        // BƯỚC 1 & 2: KIỂM TRA CACHE & DUYỆT CÂY TRIE
        // ==========================================
        bool isCacheHit = cache.get(input, suggestions);

        if (isCacheHit) {
            std::cout << " [System] LAY TU CACHE (Truy xuat O(1))!\n";
        } else {
            std::cout << " [System] Cache Miss. Dang tim kiem trong he thong...\n";
            suggestions = engine.autocomplete(input, 5);

            // Kich hoat Fuzzy Search neu Trie khong co ket qua
            if (suggestions.empty()) {
                isFuzzy = true;
                for (const auto& dictWord : fullDictionary) {
                    if (std::abs((int)dictWord.length() - (int)input.length()) <= 2) {
                        int dist = SpellChecker::calculateLevenshtein(input, dictWord);
                        if (dist <= 2) {
                            fuzzyResults.push_back({dist, dictWord});
                        }
                    }
                }
                std::sort(fuzzyResults.begin(), fuzzyResults.end());
                
                // Lay top 3 ket qua fuzzy cho vao suggestions de luu vao Cache
                int count = 0;
                for (const auto& pair : fuzzyResults) {
                    if (count++ >= 3) break;
                    suggestions.push_back(pair.second);
                }
            }

            // Luu ket qua tim duoc (tu Trie hoac Fuzzy) vao Cache
            if (!suggestions.empty()) {
                cache.put(input, suggestions);
            }
        }
        
        auto endQuery = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> queryTime = endQuery - startQuery;

        // ==========================================
        // BƯỚC 3: IN KẾT QUẢ GỢI Ý (AUTOCOMPLETE / FUZZY)
        // ==========================================
        if (!isFuzzy || isCacheHit) {
            std::cout << " * Cac goi y tim kiem:\n";
            for (const auto& w : suggestions) {
                std::cout << "   + " << w << "\n";
            }
        } else {
            if (fuzzyResults.empty()) {
                std::cout << " x Khong tim thay ket qua nao phu hop.\n";
            } else {
                std::cout << " ? Y cua ban la:\n";
                for (const auto& w : suggestions) {
                    std::cout << "   + " << w << "\n";
                }
            }
        }

        // ==========================================
        // BƯỚC 4: TÌM KIẾM TÀI LIỆU (INVERTED INDEX)
        // ==========================================
        std::vector<std::string> docs = docIndex.search(input);
        if (!docs.empty()) {
            std::cout << "\n * Tai lieu chua tu khoá '" << input << "':\n";
            for (const auto& d : docs) {
                std::cout << "   -> " << d << "\n";
            }
        }

        std::cout << " (Thoi gian truy van tong cong: " << queryTime.count() << " ms)\n";
        std::cout << "--------------------------------------------------\n";
    }
    return 0;
}