
#ifndef TEST_FUNCTION_H
#define TEST_FUNCTION_H

#include "string"

class Function {
    /**
     * questa classe permette il trim delle stringhe
     */
public:
    Function() {}

    static std::string trim(std::string &str) {
        // Trim dalla parte sinistra
        size_t start = str.find_first_not_of(" \t\n\r\f\v");
        // Trim dalla parte destra
        size_t end = str.find_last_not_of(" \t\n\r\f\v");

        // Se la stringa è vuota o solo spazi bianchi, restituiamo una stringa vuota
        if (start == std::string::npos || end == std::string::npos) {
            return "";
        }

        // Restituiamo la sottostringa tra start ed end
        return str.substr(start, end - start + 1);
    }
};

#endif //TEST_FUNCTION_H
