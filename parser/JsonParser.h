
#ifndef JSONMANAGER_JSONPARSER_H
#define JSONMANAGER_JSONPARSER_H

#include <iostream>
#include "vector"
#include <fstream>  // Aggiungi questa linea se non presente
#include"../model/Worker.h"
#include "../libs/json.hpp"  // Libreria json.hpp
#include "error.h"
#include "../utils/JsonParamFeatures.h"

using namespace nlohmann;
using namespace std;

class JsonParser {

public:
/**
 * questo metodo analizza il Json e carica tutto dentro un vector
 * ecco l'ordine   {
 *"name":"xx",
 *"surname":"xx",
 *"tax_code":"xxxxxxxxxxxxxxxx",
 *"age":22,
 *"assignment":"php developer"
 *}
 * @return vector<Worker> che servono per il file Storage
 */
    vector<Worker> analyze(string &filePath) {

        vector<Worker> workers;  // Vettore per memorizzare i lavoratori

        json j = loadJsonFromFile(filePath);  //estrazione del Json

        if (!j.is_array()) {
            throw runtime_error("Il file JSON non è un array!");
        }

        // Itera su ogni elemento nell'array
        for (auto &element: j) {
            // Assicurati che ogni campo necessario sia presente
            string name = element.contains(PARAM_NAME) ? element[PARAM_NAME] : "";
            string surname = element.contains(PARAM_SURNAME) ? element[PARAM_SURNAME] : "";
            string tax_code = element.contains(PARAM_TAXCODE) ? element[PARAM_TAXCODE] : "";
            int age = 0;  // Valore predefinito
            if (element.contains(PARAM_AGE) && element[PARAM_AGE].is_number_integer()) {
                age = element[PARAM_AGE].get<int>();  // Usa .get<int>() per estrarre un intero
            }
            string assignment = element.contains(PARAM_ASSIGNMENT) ? element[PARAM_ASSIGNMENT] : "";


            Worker worker(name, surname, tax_code, age, assignment);
            workers.push_back(worker);

        }

        return workers;

    }

    /**
     * crea un obj json e lo scrive sul json,
     * questo metodo viene chiamato dopo ogni operazione di SCRITTURA su una lista X
     * @param workers [vector]
     * @param filePath [string]
     */
    void writeWorkersToJson(const vector<Worker>& workers,string& filePath) {
        json workersJson;

        for (Worker worker : workers) {
            json workerJson;
            workerJson[PARAM_NAME] = worker.getName();
            workerJson[PARAM_SURNAME] = worker.getSurname();
            workerJson[PARAM_TAXCODE] = worker.getTaxCode();
            workerJson[PARAM_AGE] = worker.getAge();
            workerJson[PARAM_ASSIGNMENT] = worker.getAssignment();
            workersJson.push_back(workerJson);
        }

        ifstream inputFile(filePath);
        json j= loadJsonFromFile(filePath);

        if (inputFile.is_open()) {
            inputFile >> j;
        } else {
            j = json::array();
        }

        j.insert(j.end(), workersJson.begin(), workersJson.end());

        inputFile.close();

        ofstream outputFile(filePath);
        outputFile << j.dump(4);
    }

    /**
     * crea un obj vuoto [] e lo scrive sul json
     * @param filePath [string]
     */
    void clearJson(string& filePath){
        // Crea un oggetto JSON vuoto
        json emptyJson = json::array();  // Array JSON vuoto

        // Apre il file per scrivere nel JSON
        std::ofstream outputFile(filePath);

        // Scrive il JSON vuoto nel file
        outputFile << emptyJson.dump(4);  // La formattazione "pretty" (4 spazi) è opzionale
        outputFile.close();
    }

private:
    /**
     * metodo chiamato solo all'inizio per estrarre il json da un path,
     * si occupa di rimuovere (se presenti),i doppi apici dal path
     * @param filePath
     * @return
     */
    json loadJsonFromFile(string &filePath) {
        // Rimuove i doppi apici all'inizio e alla fine del percorso, se presenti
        string result = filePath;
        result=Function::trim(result);
        if (!result.empty() && result.front() == '"') {
            result.erase(0, 1);  // Rimuove il primo carattere
        }

        if (!result.empty() && result.back() == '"') {
            result.erase(result.size() - 1, 1);  // Rimuove l'ultimo carattere
        }

        filePath = result;  // Modifica il percorso originale

        // Apre il file JSON
        ifstream inputFile(filePath);

        if (!inputFile.is_open()) {
            throw runtime_error("Errore nell'aprire il file");
        }

        json j;
        inputFile >> j;  // Carica il contenuto del file JSON

        return j;  // Restituisce l'oggetto JSON
    }





};

#endif //JSONMANAGER_JSONPARSER_H
