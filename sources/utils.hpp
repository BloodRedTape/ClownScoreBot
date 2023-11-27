#pragma once

#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <unordered_map>

std::string ToLowerCase(std::string str);

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
