#include <iostream>
#include "storage/Storage.h"

string inputUser();

/**
 * main per interazione con il json
 * @return
 */
int main() {

    cout << "\n\nCiao,benvenuto in JsonManager,qui potrai gestire comodamente il tuo Json!!\n" << endl;
    Storage storage;
    string path;
    string name;
    string surname;
    string tax_code;
    int age = 0;
    string assignment;
    string search;

    while (true) {
        try {
            cout << "\nInserisci il path del file json : ";
            getline(cin,path);
            storage = Storage(path);
            break; //in caso il path sia corretto,esce
        } catch (exception e) {
            cerr << e.what();
        }
    }
    cout << "\n";

    cout << "e) esci" << endl;
    cout << "k) svuota tutto il json" << endl;
    cout << "p) stampa il contenuto del json" << endl;
    cout << "n) nuovo lavoratore" << endl;
    cout << "m) modifica un lavoratore" << endl;
    cout << "r) rimuovi un lavoratore" << endl;
    cout << "c) cerca un lavoratore" << endl;
    cout << "h) un saluto da tutti i lavoratori!!" << endl;
    cout << "\n";


    char choice;

    do {
        try{
            cout << "\nInserisci scelta : ";
            cin >> choice;
            cout << endl;
            getchar();  //terminazione buffer
            switch (choice) {
                case 'e':
                    break;
                case 'k':
                    storage.clear();
                    cout << "Json svuotato" << endl;
                    break;
                case 'p':
                    cout << storage.printAll();
                    break;
                case 'n':
                    cout << "\nnome: ";
                    getline(cin, name);

                    cout << "\ncognome: ";
                    getline(cin, surname);

                    cout << "\ncodice fiscale: ";
                    getline(cin, tax_code);  //permete la lettura con gli spazi

                    cout << "\neta: ";
                    cin >> age;

                    getchar();

                    cout << "\nruolo: ";
                    getline(cin, assignment);

                    storage.newWorker(name, surname, tax_code, age, assignment);
                    break;
                case 'm':
                    search=inputUser();
                    cout << "inserisci il nuovo ruolo: ";
                    getline(cin, assignment);
                    storage.edit(search,assignment);
                    break;
                case 'r':
                    storage.remove(inputUser());
                    break;
                case 'c':
                    storage.search(inputUser()) !=-1 ? cout<<"lavoratore in lista":cout<<"lavratore non in lista";
                    break;
                case 'h':
                    cout<<storage.helloByAll()<<endl;
                    break;
                default:
                    cerr << "\nScelta non valida" << endl;
                    break;
            }
        }catch (exception &e){
            cout<<e.what();
        }

    } while (choice != 'e');


    return 0;
}

 string inputUser(){
    string s;
    cout<<" puoi cercare il worker per nome/cognome/codice fiscale : ";
    getline(cin,s);
    return s;
}