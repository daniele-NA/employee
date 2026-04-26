
#ifndef JSONMANAGER_STORAGE_H
#define JSONMANAGER_STORAGE_H

#include <iostream>
#include <fstream>
#include "vector"
#include"../model/Worker.h"
#include "../libs/json.hpp"  // Libreria json.hpp
#include "error.h"
#include "../utils/JsonParamFeatures.h"
#include "../utils/Function.h"
#include "../parser/JsonParser.h"

using namespace std;
/**
 * la classe permette lo storage dei workers e i metodi ceh proteggono la classe JsonParser
 */
class Storage {
private:
    /**
     * field protetto da metodi che si carica dentro i dati del json
     */
    vector<Worker> workers;
protected:
    JsonParser jsonParser; //limitato allo Storage.cpp
    string current_file_path; //path del file corrente

    /**
     * metodo che viene richiamato ad ogni operazione di SCRITTURA sui dati
     */
    void write();

public:

    /**
     * si andrà ad esaminare il path,per poi caricare tutto nella classe vector
     * @param filePath per il path del file json
     */
    Storage(string &filePath);

     /**
      * costruttore vuoto per la split tra dichiarazione e assegnazione
      */
    Storage();

    /**
     * permette la stampa di tutti i parametri
     * @return string
     */
    string printAll();

    string helloByAll();

    /**
     * pulisce il json ed ovvimente il vettore
     */
    void clear();

    /**
     * nuovo lavoratore
     * @param name [string]
     * @param surname [string]
     * @param tax_code [string]
     * @param age [int]
     * @param assignment [string]
     */
    void newWorker(string name, string surname, string tax_code, int age, string assignment);

    /**
     * modifica del ruolo del lavoratore,tutto il resto è univoco
     * @param str [string]
     * @param new_assignment [string]
     */
    void edit(string str,string new_assignment);

    /**
     * si rimuove il lavoratore dalla lista e si riscrive sul json
     * @param str [string]
     */
    void remove(string str);

    /**
     *
     * @param str [string]
     * @return [int] , l'indice della posizione ? -1=non trovato : >=0 = trovato
     */
    int search(string str);


};

#endif //JSONMANAGER_STORAGE_H
