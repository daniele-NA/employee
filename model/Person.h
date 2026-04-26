#ifndef JSONMANAGER_PERSON_H
#define JSONMANAGER_PERSON_H

#include <stdexcept>
#include <string>
#include <utility>
#include "../utils/Function.h"

using namespace std;

/**
 * Abstract base class for any person-like entity managed by the program.
 *
 * Invariants enforced by the constructor:
 *  - name, surname and tax_code are non-empty (after trim)
 *  - tax_code is exactly 16 characters long
 *  - age is in [18, 65)
 *
 * The only mutable field is age — name/surname/tax_code identify the person
 * and have no setter. Subclasses may add their own mutable fields.
 *
 * @author Daniele
 */
class Person {
private:
    string name;
    string surname;
    string tax_code;   // Acts as the unique identifier for searches.
    int    age;

public:
    /**
     * Validates and assigns each field. Strings are trimmed before validation.
     * @throws runtime_error if any invariant is violated.
     */
    Person(string n, string s, string tc, int a) {
        n  = Function::trim(n);
        s  = Function::trim(s);
        tc = Function::trim(tc);

        checkStr(n);
        checkStr(s);
        checkStr(tc);
        checkAge(a);

        if (tc.size() != 16) {
            throw runtime_error("Invalid tax code (must be 16 characters)");
        }

        name     = std::move(n);
        surname  = std::move(s);
        tax_code = std::move(tc);
        age      = a;
    }

    virtual ~Person() = default;

    /** Reusable validator: rejects empty strings. */
    void checkStr(const string &s) const {
        if (s.empty()) {
            throw runtime_error("Empty string is not allowed");
        }
    }

    /** Reusable validator: enforces a working-age range. */
    void checkAge(int a) const {
        if (a < 18 || a >= 65) {
            throw runtime_error("Age must be between 18 and 64");
        }
    }

    const string &getName()    const { return name; }
    const string &getSurname() const { return surname; }
    const string &getTaxCode() const { return tax_code; }
    int           getAge()     const { return age; }

    /** Validated setter — keeps the [18, 65) invariant on every update. */
    void setAge(int newAge) {
        checkAge(newAge);
        age = newAge;
    }

    /** Render the person as a human-readable line. */
    virtual string toString() const = 0;

    /** Render a short greeting from this person. */
    virtual string hello() const = 0;
};

#endif // JSONMANAGER_PERSON_H
