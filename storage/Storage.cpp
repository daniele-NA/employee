
#include <utility>
#include"../model/Worker.h"
#include "../libs/json.hpp"  // Libreria json.hpp
#include "../utils/JsonParamFeatures.h"
#include "../parser/JsonParser.h"
#include "../storage/Storage.h"

using namespace std;

/**
 * implementazione dello storage.h ,
 * tutta la descrizione dei metodi si trova nel .h
 * @param filePath
 */

Storage::Storage(string &filePath) {

    /**
     * si occupa la classe Jsonparser di analizzare il json
     */
    workers = jsonParser.analyze(filePath);

    //se tutto va a buon fine,si imposta il path
    current_file_path = filePath;
}

// costruttore vuoto per la split
Storage::Storage() {}

string Storage::printAll() {
    string rst;
    for (auto &worker: workers) {
        rst += worker.toString();
    }
    return rst;
}

string Storage::helloByAll() {
    string rst;
    for (auto &worker: workers) {
        rst += worker.hello();
    }
    return rst;
}


void Storage::write() {
    jsonParser.clearJson(current_file_path);
    jsonParser.writeWorkersToJson(workers, current_file_path);
}

void Storage::clear() {
    workers.clear(); //svuota anche la lista ovviamente
    jsonParser.clearJson(current_file_path);
}

void Storage::newWorker(string name, string surname, string tax_code, int age, string assignment) {
    Worker w = {std::move(name), std::move(surname), std::move(tax_code), age, std::move(assignment)};
    workers.push_back(w);
    write();
}

void Storage::edit(string str, string new_assignment) {
    int index = search(str);
    if (index == -1) {
        throw runtime_error("lavoratore non trovato");
    }
    workers[index].setAssignment(new_assignment);
    write();
}

void Storage::remove(std::string str) {
    int index = search(str);
    if (index == -1) {
        throw runtime_error("lavoratore non trovato");
    }
    workers.erase(workers.begin() + index);
    write();
}

int Storage::search(std::string str) {
    str = Function::trim(str);
    for (int i = 0; i < workers.size(); ++i) {
        Worker w = workers[i];
        if (w.getName() == str || w.getSurname() == str || w.getTaxCode() == str) {
            return i;
        }
    }
    return -1;
}



