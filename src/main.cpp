#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <cctype>
#include <cmath>
#include <algorithm>
#include "../include/Trie.h"
#include "../include/SpellChecker.h"
#include "SpellChecker.cpp"
// Hàm chuẩn hóa: Chuyển thành chữ thường và chỉ giữ lại chữ cái
std::string normalizeString(const std::string& input) {
    std::string result = "";
    for (char c : input) {
        // Chỉ lấy các ký tự là chữ cái (a-z, A-Z)
        if (std::isalpha(c)) {
            result += std::tolower(c);
        }
    }
    return result;
}
int main() {
    Trie engine;
    std::string word;
    int wordCount = 0;

    std::vector<std::string> fullDictionary;

    std::cout << "[System] Đang tải từ điển vào bộ nhớ..." << std::endl;
    auto startLoad = std::chrono::high_resolution_clock::now();

    // Bạn tải một file dictionary.txt chứa khoảng 100,000 từ tiếng anh
    std::ifstream file("data/dictionary.txt"); 
    if (!file.is_open()) {
        std::cerr << "[Error] Khong the mo file data/dictionary.txt!\n";
        std::cerr << "Hay dam bao ban da tao thu muc 'data' va file 'dictionary.txt'.\n";
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
    std::cout << "Da tai " << wordCount << " tu. Thoi gian: " << loadTime.count() << " ms\n\n";
    std::cout << "==================================================\n\n";

    // Demo chức năng
    std::string rawInput;
    while (true) {
        std::cout << "--> Nhap tu khoa can tim (hoac go 'exit' de thoat): ";
        std::cin >> rawInput;

        // Neu nguoi dung go exit thi dung chuong trinh
        if (rawInput == "exit") {
            std::cout << "Tam biet!\n";
            break;
        }
        std::string input = normalizeString(rawInput);
        if (input.empty()) continue;
        // Do luong thoi gian tim kiem Autocomplete
        auto startQuery = std::chrono::high_resolution_clock::now();
        std::vector<std::string> suggestions = engine.autocomplete(input, 5);
        bool isFuzzy = false;
        std::vector<std::pair<int, std::string>> fuzzyResults;
        // Hien thi ket qua
        if (suggestions.empty()) {
            isFuzzy = true;
            for (const auto& dictWord : fullDictionary) {
                // Toi uu: Chi tinh Levenshtein neu do dai lech nhau khong qua 2 ky tu
                if (std::abs((int)dictWord.length() - (int)input.length()) <= 2) {
                    int dist = SpellChecker::calculateLevenshtein(input, dictWord);
                    // Chi lay cac tu phai sua toi da 2 loi (Edit Distance <= 2)
                    if (dist <= 2) {
                        fuzzyResults.push_back({dist, dictWord});
                    }
                }
            }
            // Sap xep uu tien tu it loi nhat (khoang cach Levenshtein nho nhat) len tren
            std::sort(fuzzyResults.begin(), fuzzyResults.end());
        } 
        auto endQuery = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> queryTime = endQuery - startQuery;
        if (!isFuzzy) {
            std::cout << " * Cac goi y tim kiem:\n";
            for (const auto& w : suggestions) {
                std::cout << "   + " << w << "\n";
            }
        } else {
            if (fuzzyResults.empty()) {
                std::cout << " x Khong tim thay ket qua nao phu hop.\n";
            } else {
                std::cout << " ? Y cua ban la:\n";
                // In ra top 3 tu gan dung nhat
                int count = 0;
                for (const auto& pair : fuzzyResults) {
                    if (count++ >= 3) break;
                    std::cout << "   + " << pair.second << " (Sai " << pair.first << " ky tu)\n";
                }
            }
        }
        std::cout << " (Thoi gian truy van: " << queryTime.count() << " ms)\n";
        std::cout << "--------------------------------------------------\n";
    }
    return 0;
}