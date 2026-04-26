#include "Storage.h"

#include <algorithm>
#include <climits>
#include <iomanip>
#include <map>
#include <sstream>
#include <utility>

#include "../utils/Function.h"

using namespace std;

Storage::Storage() = default;

Storage::Storage(const string &filePath) {
    // Normalize the path once and remember it, so all later writes
    // target the same file even if the caller mutates its own copy.
    current_file_path = JsonParser::normalizePath(filePath);
    workers           = jsonParser.analyze(current_file_path);
}

void Storage::write() {
    jsonParser.writeWorkersToJson(workers, current_file_path);
}

// ----------------------------------------------------------------- views

string Storage::printAll() const {
    if (workers.empty()) return "(no workers)\n";
    string out;
    for (const auto &w : workers) out += w.toString();
    return out;
}

string Storage::helloByAll() const {
    if (workers.empty()) return "(no workers to greet)\n";
    string out;
    for (const auto &w : workers) out += w.hello();
    return out;
}

string Storage::statistics() const {
    if (workers.empty()) return "No workers loaded.\n";

    double          sumAge = 0.0;
    int             minAge = INT_MAX;
    int             maxAge = INT_MIN;
    map<string,int> byAssignment;

    for (const auto &w : workers) {
        const int a = w.getAge();
        sumAge += a;
        if (a < minAge) minAge = a;
        if (a > maxAge) maxAge = a;
        byAssignment[w.getAssignment()]++;
    }

    ostringstream oss;
    oss << fixed << setprecision(2);
    oss << "=== Statistics ===\n";
    oss << "Total workers: " << workers.size() << "\n";
    oss << "Average age:   " << (sumAge / static_cast<double>(workers.size())) << "\n";
    oss << "Min age:       " << minAge << "\n";
    oss << "Max age:       " << maxAge << "\n";
    oss << "By assignment:\n";
    for (const auto &kv : byAssignment) {
        oss << "  - " << kv.first << ": " << kv.second << "\n";
    }
    return oss.str();
}

// --------------------------------------------------------- mutating ops

void Storage::clear() {
    workers.clear();
    jsonParser.clearJson(current_file_path);
}

void Storage::newWorker(string name, string surname, string tax_code, int age, string assignment) {
    // Build (and validate) the worker before touching the vector.
    Worker w(std::move(name), std::move(surname), std::move(tax_code),
             age, std::move(assignment));

    // Tax code is the unique identifier — refuse duplicates.
    for (const auto &existing : workers) {
        if (existing.getTaxCode() == w.getTaxCode()) {
            throw runtime_error("A worker with this tax code already exists");
        }
    }

    workers.push_back(std::move(w));
    write();
}

void Storage::edit(const string &key, const string &new_assignment) {
    const int idx = search(key);
    if (idx == -1) throw runtime_error("Worker not found");
    workers[idx].setAssignment(new_assignment);
    write();
}

void Storage::updateAge(const string &key, int new_age) {
    const int idx = search(key);
    if (idx == -1) throw runtime_error("Worker not found");
    workers[idx].setAge(new_age);
    write();
}

void Storage::remove(const string &key) {
    const int idx = search(key);
    if (idx == -1) throw runtime_error("Worker not found");
    workers.erase(workers.begin() + idx);
    write();
}

void Storage::sortBy(const string &fieldRaw) {
    const string field = Function::toLower(Function::trim(fieldRaw));

    if (field == "name") {
        std::sort(workers.begin(), workers.end(),
                  [](const Worker &a, const Worker &b) { return a.getName() < b.getName(); });
    } else if (field == "surname") {
        std::sort(workers.begin(), workers.end(),
                  [](const Worker &a, const Worker &b) { return a.getSurname() < b.getSurname(); });
    } else if (field == "age") {
        std::sort(workers.begin(), workers.end(),
                  [](const Worker &a, const Worker &b) { return a.getAge() < b.getAge(); });
    } else if (field == "assignment" || field == "role") {
        std::sort(workers.begin(), workers.end(),
                  [](const Worker &a, const Worker &b) { return a.getAssignment() < b.getAssignment(); });
    } else {
        throw runtime_error("Unknown sort field. Use: name | surname | age | assignment");
    }
    write();
}

// --------------------------------------------------------------- lookup

int Storage::search(const string &keyRaw) const {
    const string key = Function::trim(keyRaw);
    if (key.empty()) return -1;
    for (size_t i = 0; i < workers.size(); ++i) {
        const Worker &w = workers[i];
        if (Function::icontains(w.getName(),    key) ||
            Function::icontains(w.getSurname(), key) ||
            Function::icontains(w.getTaxCode(), key)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

vector<string> Storage::find(const string &keyRaw) const {
    const string key = Function::trim(keyRaw);
    vector<string> results;
    if (key.empty()) return results;
    for (const auto &w : workers) {
        if (Function::icontains(w.getName(),    key) ||
            Function::icontains(w.getSurname(), key) ||
            Function::icontains(w.getTaxCode(), key)) {
            results.push_back(w.toString());
        }
    }
    return results;
}

vector<string> Storage::filterByAssignment(const string &assignmentRaw) const {
    const string a = Function::trim(assignmentRaw);
    vector<string> results;
    if (a.empty()) return results;
    for (const auto &w : workers) {
        if (Function::icontains(w.getAssignment(), a)) {
            results.push_back(w.toString());
        }
    }
    return results;
}
