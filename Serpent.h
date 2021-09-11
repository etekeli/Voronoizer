#pragma once
#include <vector>
#include "Sommet.h"
#include "Vecteur2D.h"
#include "Graphe.h"
#include "Triangulator.h"
#include "Color.h"
#include <FileLoader.h>
#include <Voronoizer.h>

#define MAX 100000

class Serpent {
public:
	vector<Sommet<Vecteur2D>*>* germes;
	vector<Face<Color*, Color*>*>* faces;
	Face<Color*, Color*>* contour;
	Graphe<Color*, Vecteur2D>* graphe;
	Color* edge_color, * face_color, *border_color;
	

	Serpent() {
		edge_color = new Color(0, 255, 0, 0);
		face_color = new Color(0, 0, 0, 0);
		border_color = new Color(0, 255, 0, 0);
		germes = new vector<Sommet<Vecteur2D>*>();
		graphe = new Graphe<Color*, Vecteur2D>();
		faces = new vector<Face<Color*, Color*>*>;
		charger_geometries();
	}

private:
	void charger_geometries() {
		charger_contour();
		//charger_ecailles();
		charger_ecailles_voronoi();
		faces->push_back(contour);
	}

	void charger_contour() {
		FileLoader f(".\\ressources\\Nuage_contour.txt");
		for (Vecteur2D *v : (*f.listeSommets))
			germes->push_back(graphe->creeSommet(*v));

		vector<ArcTU<Color*, Color*>*> arcs_contours;

		int i = 0;
		for (Sommet<Vecteur2D>* s : (*germes)){
			int j = (i + 1) % germes->size();
			Arete<Color*, Vecteur2D>* a = graphe->creeArete(edge_color, (*germes)[i], (*germes)[j]);
			arcs_contours.push_back(new ArcTU<Color*, Color*>(a, true));
			i++;
		}

		contour = new Face<Color*, Color*>(arcs_contours, NULL);
	}

	void charger_ecailles() {
		FileLoader f(".\\ressources\\Nuage_noyaux_ecailles.txt");
		for (Vecteur2D* v : (*f.listeSommets))
			germes->push_back(graphe->creeSommet(*v));

		Triangulator<Color*, Color*> triangulator;
		faces = (vector<Face<Color*, Color*>*>*) triangulator.triangulate(germes, graphe);
		clipping(faces);
		colorier();
	}
	
	void charger_ecailles_voronoi() {
		FileLoader f(".\\ressources\\Nuage_noyaux_ecailles.txt");
		for (Vecteur2D* v : (*f.listeSommets))
			germes->push_back(graphe->creeSommet(*v));

		Triangulator<Color*, Color*>* triangulator = new Triangulator<Color*, Color*>;
		vector<Triangle<Color*, Color*>*>* triangulation = triangulator->triangulate(germes, graphe);
		triangulator->contour = this->contour;
		
		bool fait[MAX] = { false };

		clipping((vector<Face<Color*, Color*>*>*)triangulation);
		
		Voronoizer<Color*, Color*> voronoizer;
		faces = (vector<Face<Color*, Color*>*>*) voronoizer.voronoize2(triangulator, triangulation, graphe);

		colorier();
	}

	void colorier() {
		for (Face<Color*, Color*>* f : (*faces)) {
			f->v = face_color;
			for (ArcTU<Color*, Color*>* arc : f->arcs)
				arc->arete->v = edge_color;
		}
	}

	void clipping(vector<Face<Color*, Color*>*>* faces) {
		bool fait[MAX] = { false };

		for (auto it = faces->begin(); it != faces->end(); ) {

			Face<Color*, Color*>* f =  *it;
			Vecteur2D centre;

			// Calcul du centre de gravité du triangle
			for (ArcTU<Color*, Color*>* arc : (f->arcs)) {
				centre += arc->debut()->v;
			}
			centre /= f->arcs.size();

			int nbIntersection = 0;
			double s1;
			double t1;
			for (ArcTU<Color*, Color*>* a : contour->arcs)
				if (Geometrie::intersectionSegmentSegment(centre, Vecteur2D(centre.x, MAX), a->debut()->v, a->fin()->v, t1, s1))
					nbIntersection++;

			if (nbIntersection % 2 == 0) {
				delete* it;
				it = faces->erase(it);
			}
			else ++it;
		}
	}
};