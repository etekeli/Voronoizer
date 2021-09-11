#include <iostream>
#include <stdio.h>
#include <string>

#include <vector>
#include <stdio.h>
#include "Graphe.h"
#include "Vecteur2D.h"
#include "Triangulator.h"
#include "GUI.h"
#include "Serpent.h"


using namespace std;

int main(int argc, char** argv)
{

	//-------------- on dessine la triangulation -----------------------
	Serpent serpent;

	GUI gui(argc, argv);

	gui.dessiner(serpent.faces, NULL);
	return 0;

}