#include "Worker.h"
#include "../utils/Function.h"
#include <utility>

/**
 * Forwards identity fields to Person and initializes the role.
 * The assignment is trimmed and validated for emptiness.
 */
Worker::Worker(string name, string surname, string tax_code, int age, string assignmentRaw)
    : Person(std::move(name), std::move(surname), std::move(tax_code), age),
      assignment(Function::trim(assignmentRaw)) {
    checkStr(assignment);
}

string Worker::toString() const {
    return "Name: " + getName()
         + ", Surname: " + getSurname()
         + ", Tax code: " + getTaxCode()
         + ", Age: " + std::to_string(getAge())
         + ", Assignment: " + assignment + "\n";
}

string Worker::hello() const {
    return "Hi, I'm " + getName() + " " + getSurname()
         + " and I'm a " + assignment + "!\n";
}

const string &Worker::getAssignment() const {
    return assignment;
}

/**
 * Validates the *new* value (the original code mistakenly trimmed and
 * checked the old field, then silently overwrote it without validation).
 */
void Worker::setAssignment(const string &newAssignment) {
    string a = Function::trim(newAssignment);
    checkStr(a);
    assignment = std::move(a);
}
