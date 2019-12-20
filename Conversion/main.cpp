/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: polydord
 *
 * Created on 29 novembre 2019, 14:55
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
/*
 * 
 */
using namespace std;

int main() {
    ofstream fichierTxt("/users/info/etu-s3/polydord/projet/exempleTrad.txt");
    ifstream fichierCsv("/users/info/etu-s3/polydord/projet/exemple222.csv");
    if (fichierTxt && fichierCsv) { //Si les deux fichiers ont été ouverts 
        string ligne; //on lit ligne par ligne
        bool ok = false;
        while (getline(fichierCsv, ligne)) { //tant qu'il y a des lignes
            if (ligne.at(0) == 'A' && ligne.at(1) == 'R') //tant qu'on est pas à la 1ere ligne du tableau on n'écrit pas
                ok = true;
            if (ok == true) { //si on est à la premiere ligne du tableau on écrit
                int i = 0;
                while(i < ligne.length()){
                    //ligne.erase(0, 3);
                //for (int i = 0; i < ligne.length()) { //remplace les séparateurs ; en séparateurs ','
                    if (ligne.at(i) == ';') {
                            ligne.replace(i, 1, "','");
                            i++;
                        }
                    else{
                        i++;
                    }
                }
                fichierTxt << "INSERT INTO adherent (nom, prenom, categorie, role, datenaissance, adresse, telephone, mail, numLicence, urlPhoto, urlPhotoAlt) Values('" << ligne << "');" << endl;
            }
        }
    } else {
        cout << "erreur" << endl;
    }
    //jour / mois / année
    //année / mois / jour
    return 0;
}


