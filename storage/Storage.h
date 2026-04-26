#ifndef JSONMANAGER_STORAGE_H
#define JSONMANAGER_STORAGE_H

#include <string>
#include <vector>
#include "../model/Worker.h"
#include "../parser/JsonParser.h"

using namespace std;

/**
 * In-memory collection of Worker plus all the business operations
 * exposed to the UI. Every mutation is flushed to disk immediately
 * via JsonParser, so the file on disk is always in sync with this
 * vector after a successful operation.
 */
class Storage {
private:
    vector<Worker> workers;       // Authoritative in-memory collection.
    JsonParser     jsonParser;    // I/O helper.
    string         current_file_path;

    /** Persists the current vector to disk (full overwrite). */
    void write();

public:
    Storage();

    /**
     * Loads workers from @p filePath. The path is normalized once and
     * stored, so subsequent writes do not require the caller to keep it.
     */
    explicit Storage(const string &filePath);

    // ----- read-only views -----

    /** Multi-line dump of every worker. */
    string printAll() const;

    /** One greeting per worker, concatenated. */
    string helloByAll() const;

    /** Aggregate metrics: count, age range, role distribution. */
    string statistics() const;

    /** Number of workers currently loaded. */
    size_t size() const { return workers.size(); }

    // ----- mutating operations (each persists to disk on success) -----

    /** Empties both the in-memory vector and the file. */
    void clear();

    /**
     * Appends a brand new worker. Tax codes are unique: a duplicate
     * raises a runtime_error and the file is left untouched.
     */
    void newWorker(string name, string surname, string tax_code, int age, string assignment);

    /** Updates the assignment of the first worker matching @p key. */
    void edit(const string &key, const string &new_assignment);

    /** Updates the age of the first worker matching @p key. */
    void updateAge(const string &key, int new_age);

    /** Removes the first worker matching @p key. */
    void remove(const string &key);

    /**
     * Re-orders workers by the given @p field
     * (one of: "name", "surname", "age", "assignment" / "role").
     */
    void sortBy(const string &field);

    // ----- lookup helpers -----

    /**
     * Case-insensitive substring search across name/surname/tax code.
     * @return index of the first match, or -1 if none found.
     */
    int search(const string &key) const;

    /** Like search() but returns the rendered toString of every match. */
    vector<string> find(const string &key) const;

    /** Workers whose assignment contains @p assignment (case-insensitive). */
    vector<string> filterByAssignment(const string &assignment) const;
};

#endif // JSONMANAGER_STORAGE_H
