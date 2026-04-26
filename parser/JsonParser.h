#ifndef JSONMANAGER_JSONPARSER_H
#define JSONMANAGER_JSONPARSER_H

#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "../libs/json.hpp"
#include "../model/Worker.h"
#include "../utils/Function.h"
#include "../utils/JsonParamFeatures.h"

using namespace nlohmann;
using namespace std;

/**
 * Pure I/O layer: reads, writes and clears the workers JSON file.
 * Knows nothing about business rules — that responsibility belongs to Storage.
 */
class JsonParser {
public:
    /**
     * Sanitizes a path string typed by the user:
     * trims whitespace and strips a single pair of surrounding double quotes
     * (handy for users who paste paths with quotes from a file manager).
     */
    static string normalizePath(const string &filePath) {
        string r = Function::trim(filePath);
        if (!r.empty() && r.front() == '"') r.erase(0, 1);
        if (!r.empty() && r.back()  == '"') r.pop_back();
        return r;
    }

    /**
     * Loads the JSON file and converts each entry into a Worker.
     * @throws runtime_error if the file is missing, malformed,
     *         or its top-level value is not an array.
     */
    vector<Worker> analyze(const string &filePath) {
        json j = loadJsonFromFile(filePath);

        if (!j.is_array()) {
            throw runtime_error("The JSON file must contain an array of workers");
        }

        vector<Worker> workers;
        workers.reserve(j.size());
        for (const auto &el : j) {
            string name       = el.value(PARAM_NAME, string{});
            string surname    = el.value(PARAM_SURNAME, string{});
            string tax_code   = el.value(PARAM_TAXCODE, string{});
            string assignment = el.value(PARAM_ASSIGNMENT, string{});
            int    age        = 0;
            if (el.contains(PARAM_AGE) && el[PARAM_AGE].is_number_integer()) {
                age = el[PARAM_AGE].get<int>();
            }
            workers.emplace_back(std::move(name), std::move(surname),
                                 std::move(tax_code), age, std::move(assignment));
        }
        return workers;
    }

    /**
     * Replaces the file content with the serialized form of @p workers.
     * One write per call — the file is the authoritative source of truth
     * after every mutating operation in Storage.
     */
    void writeWorkersToJson(const vector<Worker> &workers, const string &filePath) {
        json arr = json::array();
        for (const auto &w : workers) {
            json e;
            e[PARAM_NAME]       = w.getName();
            e[PARAM_SURNAME]    = w.getSurname();
            e[PARAM_TAXCODE]    = w.getTaxCode();
            e[PARAM_AGE]        = w.getAge();
            e[PARAM_ASSIGNMENT] = w.getAssignment();
            arr.push_back(std::move(e));
        }

        ofstream out(filePath);
        if (!out.is_open()) {
            throw runtime_error("Cannot open file for writing: " + filePath);
        }
        out << arr.dump(4);
    }

    /** Overwrites the file with an empty JSON array. */
    void clearJson(const string &filePath) {
        ofstream out(filePath);
        if (!out.is_open()) {
            throw runtime_error("Cannot open file for writing: " + filePath);
        }
        out << json::array().dump(4);
    }

private:
    /**
     * Opens the file and parses it as JSON.
     * Wraps nlohmann::json's parse exception in a friendlier message.
     */
    json loadJsonFromFile(const string &filePath) {
        ifstream in(filePath);
        if (!in.is_open()) {
            throw runtime_error("Cannot open file: " + filePath);
        }
        try {
            json j;
            in >> j;
            return j;
        } catch (const exception &e) {
            throw runtime_error(string("Malformed JSON: ") + e.what());
        }
    }
};

#endif // JSONMANAGER_JSONPARSER_H
