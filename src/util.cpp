#include "../include/util.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

int sum(const std::vector<int> &list, int start, int size) {
    int total = 0;
    for (int i = 0; i < size; i++) {
        total += list[start + i];
    }
    return total;
}

int sum(const std::vector<int> &list) {
    int total = 0;
    for (int val : list) {
        total += val;
    }
    return total;
}

std::vector<std::string> split(const std::string &str, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;

    while (getline(ss, token, delimiter)) {
        result.push_back(token);
    }

    return result;
}

std::string strip(const std::string &str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) {
        return "";
    }
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

std::string rstrip(const std::string &str) {
    size_t end = str.find_last_not_of(" \t\n\r");
    if (end == std::string::npos) {
        return "";
    }
    return str.substr(0, end + 1);
}

std::string lstrip(const std::string &str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) {
        return "";
    }
    return str.substr(start);
}

std::string deleteLeadingLinesOrWhitespace(const std::string &str) {
    int i = 0;
    int lineStart = 0;
    bool newline = false;
    while (i < str.length()) {
        if (str[i] == ' ') {
            i++;
        }
        else if (str[i] == '\n') {
            newline = true;
            lineStart = ++i;
        }
        else {
            break;
        }
    }
    if (newline) {
        return str.substr(lineStart, str.size() - lineStart);
    }
    else if (i) {
        return str.substr(i, str.size() - i);
    }
    return str;
}

std::string repeat(const std::string &str, int count) {
    std::stringstream ss;
    for (int i = 0; i < count; ++i) {
        ss << str;
    }
    return ss.str();
}

std::string repeat(char c, int count) {
    std::stringstream ss;
    for (int i = 0; i < count; ++i) {
        ss << c;
    }
    return ss.str();
}

std::string catFile(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::exception();
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

std::string catStdin() {
    std::stringstream buffer;
    buffer << std::cin.rdbuf();
    return buffer.str();
}

std::vector<std::string> word_wrap(std::string text, int maxWidth) {
    std::string line;
    std::vector<std::string> lines;
    int i = 0;
    while (i < text.length()) {
        while (line.length() < maxWidth) {
            if (i == text.length()) {
                if (line.length()) {
                    lines.push_back(line);
                }
                return lines;
            }
            if (text[i] == '\n') {
                lines.push_back(line);
                line = "";
            }
            else if (isspace(text[i])) {
                line += ' ';
                while (isspace(text[i + 1])) {
                    i++;
                }
            }
            else {
                line += text[i];
            }
            i++;
        }
        if (isspace(text[i])) {
            lines.push_back(line);
            line = "";
            while (isspace(text[i])) {
                i++;
            }
        }
        else {
            if (i == text.length()) {
                lines.push_back(line);
                line = "";
                continue;
            }
            for (int j = line.length() - 1; j >= 0; j--) {
                if (isspace(line[j])) {
                    lines.push_back(line.substr(0, j));
                    line = line.substr(j + 1, line.length() - j);
                    break;
                }
            }
            if (line.length() >= maxWidth) {
                lines.push_back(line);
                line = "";
            }
        }
    }
    if (line.length()) {
        lines.push_back(line);
    }
    return lines;
}
