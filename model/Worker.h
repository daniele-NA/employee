#ifndef JSONMANAGER_WORKER_H
#define JSONMANAGER_WORKER_H

#include "Person.h"
#include <string>

class Worker : public Person {
private:
    string assignment;  // Parametro variabile, si può cambiare il ruolo

public:
    // Costruttore
    Worker(string name, string surname, string tax_code, int age, string assignment);

    // Override dei metodi puri virtuali della classe Person
    string toString() override;
    string hello() override;

    // Getter e Setter per 'assignment',verranno poi implementati nel worker.cpp
    const string &getAssignment() const;
    void setAssignment(const string& newAssignment);
};

#endif // JSONMANAGER_WORKER_H
