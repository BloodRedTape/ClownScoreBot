#pragma once

#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <unordered_map>

struct CStrComparator {
    bool operator()(const char * a, const char *b) const {
        return std::strcmp(a, b) == 0;
    }
};

std::string ToLowerCase(std::string str);

int CountChars(char ch, const char *string);

inline int CountChars(char ch, const std::string &string){ return CountChars(ch, string.c_str()); }

std::string ReadEntireFile(const std::string &filepath);

void WriteEntireFile(const std::string &filepath, const std::string &buffer);

template <typename T, typename E>
void PushBackUnique(T& collection, E element) {
    if (std::find(collection.begin(), collection.end(), element) != collection.end())
        return;

    collection.push_back(std::move(element));
}

template <typename C1, typename C2>
void PushBack(C1& destination, const C2& source) {
    for (const auto& e : source) {
        destination.push_back(e);
    }
}

template <typename T>
auto Keys(const T& collection) {
    using KeyType = std::remove_cv_t<std::remove_reference_t<decltype(collection.begin()->first)>>;
    std::vector<KeyType> keys;

    for (const auto& [key, value] : collection) {
        keys.push_back(key);
    }

    return keys;
}

template <typename K, typename V>
std::vector<std::pair<K, V>> ToArray(std::unordered_map<K, V> &&map) {
    std::vector<std::pair<K, V>> result;

    for (auto&& [key, value] : map) {
        result.emplace_back(std::move(key), std::move(value));
    }

    return result;
}

inline std::string Join(const std::vector<std::string>& strings, char delimiter) {
    std::string result;

    for (int i = 0; i < strings.size(); i++) {
        result += strings[i];

        if (i == strings.size() - 1) {
            break;
        }

        result += delimiter;
    }

    return result;
}

inline bool StartsWith(const std::string& str1, const std::string& str2) {
    if (str1.length() < str2.length()) {
        return false;
    }
    std::string::const_iterator it1(str1.begin());
    std::string::const_iterator end1(str1.end());
    std::string::const_iterator it2(str2.begin());
    std::string::const_iterator end2(str2.end());
    while (it1 != end1 && it2 != end2) {
        if (*it1 != *it2) {
            return false;
        }
        ++it1;
        ++it2;
    }
    return true;
}


std::string CurrentDataAndTime();
