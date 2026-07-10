#ifndef LRUCACHE_H
#define LRUCACHE_H

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <unordered_map>

class LRUCache {
private:
    int capacity;
    // Danh sách lưu trữ cặp {từ khóa, danh sách gợi ý}
    std::list<std::pair<std::string, std::vector<std::string>>> cacheList;
    
    // Map lưu con trỏ trỏ tới node trong danh sách để truy xuất O(1)
    std::unordered_map<std::string, std::list<std::pair<std::string, std::vector<std::string>>>::iterator> cacheMap;

public:
    LRUCache(int cap) : capacity(cap) {}

    // Lấy dữ liệu từ Cache
    bool get(const std::string& key, std::vector<std::string>& result) {
        if (cacheMap.find(key) == cacheMap.end()) {
            return false; // Cache miss
        }
        // Cache hit: Đưa node này lên đầu danh sách (đánh dấu là vừa được sử dụng)
        cacheList.splice(cacheList.begin(), cacheList, cacheMap[key]);
        result = cacheMap[key]->second;
        return true;
    }

    // Lưu dữ liệu vào Cache
    void put(const std::string& key, const std::vector<std::string>& value) {
        if (cacheMap.find(key) != cacheMap.end()) {
            // Đã tồn tại, cập nhật giá trị và đưa lên đầu
            cacheList.splice(cacheList.begin(), cacheList, cacheMap[key]);
            cacheMap[key]->second = value;
            return;
        }

        // Nếu Cache đầy, xóa phần tử ở cuối danh sách (ít dùng nhất)
        if (cacheList.size() == capacity) {
            auto last = cacheList.back();
            cacheMap.erase(last.first);
            cacheList.pop_back();
        }

        // Chèn phần tử mới vào đầu danh sách
        cacheList.push_front({key, value});
        cacheMap[key] = cacheList.begin();
    }
};

#endif