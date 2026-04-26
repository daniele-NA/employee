#include <exception>
#include <iostream>
#include <string>
#include "storage/Storage.h"
#include "utils/Function.h"

using namespace std;

// ---------------------------------------------------------------- helpers

static void   printMenu();
static string promptLine(const string &label);
static string promptSearchKey();
static int    promptInt(const string &label);
static void   printList(const vector<string> &items, const string &emptyMessage);

// ------------------------------------------------------------------- main

/**
 * Entry point. Drives an interactive REPL on top of Storage:
 *  - Asks the user for a JSON file path until one loads successfully.
 *  - Dispatches single-letter commands to Storage operations.
 *  - Catches and reports every exception so a single typo never kills the loop.
 */
int main() {
    cout << "\nWelcome to JsonManager — manage your workers JSON the easy way!\n" << endl;

    Storage storage;

    // Path acquisition loop: keep asking until a valid file is loaded.
    while (true) {
        try {
            const string path = promptLine("Path of the JSON file");
            storage = Storage(path);
            break;
        } catch (const exception &e) {
            cerr << "[error] " << e.what() << "\n";
        }
    }

    cout << "\nLoaded " << storage.size() << " worker(s).\n";
    printMenu();

    // Main dispatch loop. We read a whole line and look at its first
    // non-blank character — this avoids the classic cin/getline buffer
    // mismatch that plagued the original implementation.
    string line;
    while (true) {
        cout << "\nChoice (? for menu): ";
        if (!getline(cin, line)) break;

        const string trimmed = Function::trim(line);
        if (trimmed.empty()) continue;
        const char choice = trimmed[0];

        try {
            switch (choice) {
                case 'e':
                    cout << "Bye!\n";
                    return 0;

                case '?':
                    printMenu();
                    break;

                case 'k':
                    storage.clear();
                    cout << "JSON cleared.\n";
                    break;

                case 'p':
                    cout << storage.printAll();
                    break;

                case 'n': {
                    const string name       = promptLine("Name");
                    const string surname    = promptLine("Surname");
                    const string taxCode    = promptLine("Tax code");
                    const int    age        = promptInt ("Age");
                    const string assignment = promptLine("Assignment");
                    storage.newWorker(name, surname, taxCode, age, assignment);
                    cout << "Worker added.\n";
                    break;
                }

                case 'm': {
                    const string key      = promptSearchKey();
                    const string newRole  = promptLine("New assignment");
                    storage.edit(key, newRole);
                    cout << "Worker updated.\n";
                    break;
                }

                case 'a': {
                    const string key    = promptSearchKey();
                    const int    newAge = promptInt("New age");
                    storage.updateAge(key, newAge);
                    cout << "Age updated.\n";
                    break;
                }

                case 'r':
                    storage.remove(promptSearchKey());
                    cout << "Worker removed.\n";
                    break;

                case 'c':
                    printList(storage.find(promptSearchKey()),
                              "No matching worker.\n");
                    break;

                case 'f':
                    printList(storage.filterByAssignment(promptLine("Filter by assignment")),
                              "No worker with that assignment.\n");
                    break;

                case 's':
                    storage.sortBy(promptLine("Sort by [name|surname|age|assignment]"));
                    cout << "Workers sorted.\n";
                    break;

                case 'i':
                    cout << storage.statistics();
                    break;

                case 'h':
                case 'g':
                    cout << storage.helloByAll();
                    break;

                default:
                    cerr << "Invalid choice. Type ? for the menu.\n";
                    break;
            }
        } catch (const exception &e) {
            // Centralized error reporting keeps the loop responsive
            // even when individual operations throw.
            cerr << "[error] " << e.what() << "\n";
        }
    }

    return 0;
}

// --------------------------------------------------------- helpers impl

static void printMenu() {
    cout << "\n=== JsonManager menu ===\n";
    cout << " p) print all workers\n";
    cout << " n) add a new worker\n";
    cout << " m) modify a worker's assignment\n";
    cout << " a) update a worker's age\n";
    cout << " r) remove a worker\n";
    cout << " c) search a worker (case-insensitive substring)\n";
    cout << " f) filter workers by assignment\n";
    cout << " s) sort workers (name|surname|age|assignment)\n";
    cout << " i) statistics\n";
    cout << " g) greetings from all workers\n";
    cout << " k) clear the whole JSON\n";
    cout << " ?) show this menu\n";
    cout << " e) exit\n";
}

static string promptLine(const string &label) {
    cout << label << ": ";
    string s;
    getline(cin, s);
    return s;
}

static string promptSearchKey() {
    return promptLine("Search worker by name/surname/tax code");
}

/**
 * Reads an integer from stdin, retrying on garbage input.
 * Using getline + stoi avoids leaving cin in a fail state — the original
 * `cin >> age` flow could silently break the rest of the menu.
 */
static int promptInt(const string &label) {
    while (true) {
        const string raw = promptLine(label);
        try {
            size_t    pos = 0;
            const int v   = stoi(raw, &pos);
            if (pos != raw.size()) throw runtime_error("trailing characters");
            return v;
        } catch (...) {
            cerr << "Please type a valid integer.\n";
        }
    }
}

static void printList(const vector<string> &items, const string &emptyMessage) {
    if (items.empty()) {
        cout << emptyMessage;
        return;
    }
    for (const auto &s : items) cout << s;
}
