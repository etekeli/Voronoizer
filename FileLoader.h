#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <Vecteur2D.h>

using namespace std;

class FileLoader {

private:
	string _filename;

public:
    vector<Vecteur2D*>* listeSommets = new vector<Vecteur2D*>;
    FileLoader(string filename): _filename(filename){

    ifstream file(filename);
    string line, coords;


    if (file){

        while (getline(file, line)){
            istringstream ss(line);
            Vecteur2D* v = new Vecteur2D();

            getline(ss, coords, ',');
            v->x = stod(coords);
            getline(ss, coords);

            v->y = stod(coords);
            listeSommets->push_back(v);
        }
    }
    file.close();
}


};

