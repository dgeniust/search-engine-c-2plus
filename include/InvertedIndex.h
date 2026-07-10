#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

class InvertedIndex {
private:
    // Ánh xạ: Từ khóa -> Danh sách các Document ID chứa từ khóa đó
    std::unordered_map<std::string, std::vector<int>> index;
    // Lưu tên của các Document dựa trên ID
    std::unordered_map<int, std::string> docMap;
    int docIdCounter = 1;

public:
    // Giả lập việc thêm một bài viết/tài liệu vào hệ thống
    void addDocument(const std::string& docName, const std::string& content) {
        int currentDocId = docIdCounter++;
        docMap[currentDocId] = docName;

        std::stringstream ss(content);
        std::string word;
        while (ss >> word) {
            // Chỉ thêm nếu Document ID này chưa được lưu cho từ khóa này (tránh lặp ID)
            if (index[word].empty() || index[word].back() != currentDocId) {
                index[word].push_back(currentDocId);
            }
        }
    }

    // Truy xuất danh sách tài liệu chứa từ khóa
    std::vector<std::string> search(const std::string& keyword) {
        std::vector<std::string> result;
        if (index.find(keyword) != index.end()) {
            for (int id : index[keyword]) {
                result.push_back(docMap[id]);
            }
        }
        return result;
    }
};

#endif