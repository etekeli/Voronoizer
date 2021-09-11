/*#include <iostream>
#include <string>
#include "Graphe.h"
#include "Triangle.h"
#include "Cercle.h"
#include "ArcTU.h"
#include "Arete.h"
#include "Vecteur2D.h"
#include <vector>
#include <stdio.h>
#include "Voronoizer.h"
#include "GUI.h"
#include <random>


using namespace std;

int main(int argc, char** argv)
{
	//--------------- générateur de distribution uniforme -----------------
	default_random_engine generator;
	uniform_int_distribution<int> distribution(-10000, 10000);

	//-------------- voronoizer, triangulateur et variables concernées -----------------------
	Voronoizer<Color*, Color*> voronoizer;
	Graphe<Color*, Vecteur2D>* graphe = new Graphe<Color*, Vecteur2D>;
	vector<Sommet<Vecteur2D>*>* sommets = new vector<Sommet<Vecteur2D>*>;


	//-------- génération des sommets en veillant à ce qu'il n'y ait pas de duplications ----------
	for (int i = 0; i < 300; i++) {
		Vecteur2D a = Vecteur2D(distribution(generator), distribution(generator));
		//Si il n'existe aucun sommet, on l'ajoute
		if (sommets->size() == 0)
			sommets->push_back(graphe->creeSommet(a));

		//On vérifie si il existe un sommet ayant ces coordonnées
		bool existe = false;
		for (Sommet<Vecteur2D>* s : (*sommets))
			if ((s->v.x == a.x && s->v.y == a.y))
				existe = true;

		//Si il n'existe aucun sommet de même coordonnées, on l'ajoute
		if (!existe)
			sommets->push_back(graphe->creeSommet(a));
	}

	//--------- génération du diagramme de voronoi ------------
	vector<Face<Color*, Color*>*>* voronoi = voronoizer.voronoize(sommets, graphe);
	//vector< Sommet<Vecteur2D>*>* germes = voronoizer.germes;

	Triangulator<Color*, Color*> triangulator;
	vector<Triangle<Color*, Color*>*>* triangulation = triangulator.triangulate(sommets, graphe);
	vector< Sommet<Vecteur2D>*>* germes = voronoizer.germes;

	Color* col_triangulation_edge = new Color(255, 0, 0, 0);
	Color* col = new Color(0, 0, 0, 0);

	for (Face<Color*, Color*>* f : (*voronoi)) {
		for (ArcTU<Color*, Color*>* a : f->arcs) {
			a->arete->v = col;
		}
	}

	for (Triangle<Color*, Color*>* t : (*triangulation)) {
		for (ArcTU<Color*, Color*>* a : t->arcs) {
			a->arete->v = col_triangulation_edge;
		}
//		voronoi->push_back(t);
	}

	//Face<Color*, Color*>* contour = triangulator.contour;
	//voronoi->push_back(contour);



GUI gui(argc, argv);

gui.dessiner(voronoi, germes);
return 0;

}*/