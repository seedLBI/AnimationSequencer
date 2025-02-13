#include "Utils.Text.h"


std::vector<std::string> Split(const std::string& line, const std::string& delimiter) {
    std::string s = line;
    //std::string delimiter = delimiter;

    std::vector<std::string> result;

    size_t pos = 0;
    std::string token;

    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        result.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    if (s.length() != 0) {
        result.push_back(s);
    }
    return result;
}