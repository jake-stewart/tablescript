#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

template <typename T>
std::string join(const std::vector<T> &elements, const std::string &delim) {
    std::stringstream ss;
    bool first = true;

    for (const auto &element : elements) {
        if (!first) {
            ss << delim;
        }
        ss << element;
        first = false;
    }

    return ss.str();
}

std::vector<std::string> split(const std::string &str, char delimiter);
std::string strip(const std::string &str);
std::string rstrip(const std::string &str);
std::string lstrip(const std::string &str);
std::string deleteLeadingLinesOrWhitespace(const std::string &str);
std::string wrap(const std::string &str, int width);
std::string repeat(const std::string &str, int count);
std::string repeat(char c, int count);
std::vector<std::string> word_wrap(std::string text, int maxWidth);

int sum(const std::vector<int> &list, int start, int end);
int sum(const std::vector<int> &list);

std::string catFile(const std::string &path);
std::string catStdin();

#endif
