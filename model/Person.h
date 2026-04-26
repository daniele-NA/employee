
#ifndef TEST_PERSONA_H
#define TEST_PERSONA_H

#include <string>
#include <stdexcept>
#include "../utils/Function.h"  // metodi di controllo


using namespace std;

/**
 * questo file si prenderà poi le implementazioni del .cpp
 *   <blockquote><pre>
 *   ABSTRACT CLASS
 * </pre></blockquote>
 * @author Daniele
 */
class Person {
    /**
     * l'unica variabile che può cambiare è l'età
     * il resto è univoco, dato che non fornisco metodi SETTER
     */
private:
    string name;
    string surname;
    string tax_code;   //QUESTA è LA CHIAVE SULLA QUALE SI CICLA E SI FA LA RICERCA
    int age;
public:

    /**
     * controllo dei parametri e assegnazione
     * @param name [string]
     * @param surname [string]
     * @param tax_code [string]
     * @param age [int]
     */
    Person(string name, string surname, string tax_code, int age) {
        name = Function::trim(name);     // Trim del nome
        surname = Function::trim(surname); // Trim del cognome
        tax_code = Function::trim(tax_code); //trim tax_code


        checkStr(name);
        checkStr(surname);
        checkStr(tax_code);
        checkAge(age);

        if(tax_code.size()!=16){
            throw runtime_error("Codice fiscale non valido");
        }


        this->name = name;
        this->surname = surname;
        this->tax_code=tax_code;
        this->age = age;
    }

    /**
     * si mantiene questo metodo pubblico,
     * in modo da poter essere usato dalle sottoclassi
     * @param str [string]
     */
    void checkStr(const string &str) {
        if (str.empty()) {
            throw runtime_error("Stringa vuota");
        }
    }


    /**
     * si mantiene questo metodo pubblico,
     * in modo da poter essere usato dalle sottoclassi
     * @param &a [int]
     */
    void checkAge(const int &a){
        if (a < 18 || a >= 65) {
            throw runtime_error("Età non valida");
        }
    }

    const string &getName() {
        return name;
    }

    const string &getSurname() {
        return surname;
    }

    const string &getTaxCode() {
        return tax_code;
    }

    int &getAge() {
        return age;
    }


    virtual string toString() = 0;   //metodo virtuale per la stampa

    virtual string hello() = 0;   //metodo virtuale per il saluto da parte del programmatore


};

#endif //TEST_PERSONA_H
