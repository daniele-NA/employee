#include "Worker.h"
#include <stdexcept>
#include <utility>

/**
 * questo costruttore si prende carico di tutti i parametri della claae stessa
 * e della superclasse PERSON.H
 * @author daniele
 * @param name [string]
 * @param surname [string]
 * @param tax_code [string]
 * @param age [int]
 * @param assignment [string]
 */
Worker::Worker(string name, string surname, string tax_code, int age, string assignment)
        : Person(std::move(name), std::move(surname), std::move(tax_code), age), assignment(std::move(assignment)) {
    // La lista di inizializzazione chiama il costruttore della classe base Person
}

// Implementazione del metodo toString()
string Worker::toString() {
    return "Nome: " + getName() + ", Cognome: " + getSurname() + ", Codice Fiscale: " + getTaxCode() +
           ", Eta: " + std::to_string(getAge()) + ", Ruolo: " + assignment+"\n\n";
}

// Implementazione del metodo hello()
string Worker::hello() {
    return "Ciao, sono " + getName() + " " + getSurname() + " e sono un "+getAssignment()+" !!\n";
}

// Getter per assignment
const string & Worker::getAssignment() const { //implementazione del get
    return assignment;
}

// Setter per assignment
void Worker::setAssignment(const string& newAssignment) {
    assignment=Function::trim(assignment);  //utils
    checkStr(assignment);  //controllo con metodo della superclasse
    assignment = newAssignment;
}
