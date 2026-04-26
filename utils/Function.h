#ifndef JSONMANAGER_FUNCTION_H
#define JSONMANAGER_FUNCTION_H

#include <algorithm>
#include <cctype>
#include <string>

/**
 * Generic, side-effect-free string helpers used across the project.
 * Keep this header dependency-free so any module can include it cheaply.
 */
class Function {
public:
    Function() = delete;

    /** Returns a copy of @p str without leading/trailing whitespace. */
    static std::string trim(const std::string &str) {
        const std::size_t start = str.find_first_not_of(" \t\n\r\f\v");
        if (start == std::string::npos) return "";
        const std::size_t end = str.find_last_not_of(" \t\n\r\f\v");
        return str.substr(start, end - start + 1);
    }

    /** Lower-cased copy (ASCII only — adequate for our domain). */
    static std::string toLower(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return s;
    }

    /** Case-insensitive substring match. */
    static bool icontains(const std::string &haystack, const std::string &needle) {
        if (needle.empty()) return false;
        return toLower(haystack).find(toLower(needle)) != std::string::npos;
    }
};

#endif // JSONMANAGER_FUNCTION_H
