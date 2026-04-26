#ifndef JSONMANAGER_WORKER_H
#define JSONMANAGER_WORKER_H

#include <string>
#include "Person.h"

/**
 * Concrete person who has a job role ("assignment").
 * Inherits all identity fields and validation from Person.
 */
class Worker : public Person {
private:
    string assignment;  // Mutable — workers can change role over time.

public:
    Worker(string name, string surname, string tax_code, int age, string assignment);

    // Polymorphic rendering hooks declared in Person.
    string toString() const override;
    string hello()    const override;

    const string &getAssignment() const;

    /** Validated setter: trims and rejects empty values. */
    void setAssignment(const string &newAssignment);
};

#endif // JSONMANAGER_WORKER_H
