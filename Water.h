#pragma once
#include "ArcTU.h"
#include "Arete.h"
#include "Vecteur2D.h"
#include <vector>
#include <stdio.h>
#include "Voronoizer.h"
#include "GUI.h"
#include <random>

class Water {
public:
	Color* edge_color = new Color(194, 237, 236, 0);
	Color* face_color = new Color(49, 219, 222, 0);
	vector<Face<Color*, Color*>*>* faces;

	Water() {
		//--------------- générateur de distribution uniforme -----------------
		default_random_engine generator;
		uniform_int_distribution<int> distribution(-10000, 10000);

		//-------------- voronoizer, triangulateur et variables concernées -----------------------
		Voronoizer<Color*, Color*> voronoizer;
		Graphe<Color*, Vecteur2D>* graphe = new Graphe<Color*, Vecteur2D>;
		vector<Sommet<Vecteur2D>*>* sommets = new vector<Sommet<Vecteur2D>*>;


		//-------- génération des sommets en veillant à ce qu'il n'y ait pas de duplications ----------
		for (int i = 0; i < 5000; i++) {
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
		faces = voronoizer.voronoize(sommets, graphe);
		colorier();
	}

	void colorier() {
		for (Face<Color*, Color*>* f : (*faces)) {
			f->v = face_color;
			for (ArcTU<Color*, Color*>* arc : f->arcs)
				arc->arete->v = edge_color;
		}
	}
};