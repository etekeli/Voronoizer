#pragma once
#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "Sommet.h"
#include "Triangle.h"
#include "Graphe.h"
#include "Cercle.h"
#include "Face.h"
#include "Triangulator.h"
#include "Geometrie.h"

#define MAX_ARRAY 100000

using namespace std;

/*
* Classe Voronoizer
* Prends en entre une liste de sommets
* et crée un diagramme de Voronoï
*
* T est la nature de l'information porte par une arête
* S est la nature de l'information porte par une face
*/
template <class S, class T>
class Voronoizer {
public:
	Voronoizer() {
		this->germes = new vector<Sommet<Vecteur2D>*>;
		this->cellules = new vector<Face<S, T>*>;
	}
	~Voronoizer() {}

	/**
	* Retourne la liste traingule de la liste de sommets en entre
	*/
	vector<Face<S, T>*>* voronoize(vector<Sommet<Vecteur2D>*>* sommets, Graphe<T, Vecteur2D>* graphe) {
		this->graphe = graphe;
		triangulator = new Triangulator<S, T>;
		triangulation = triangulator->triangulate(sommets, graphe);
		construire_cellules();
		return this->cellules;
	}

	vector<Face<S, T>*>* voronoize2(Triangulator<S, T>* triangulator, vector<Triangle<S, T>*>* triangulation, Graphe<T, Vecteur2D>* graphe) {
		this->graphe = graphe;
		//triangulator = new Triangulator<S, T>;
		this->triangulation = triangulation;
		this->triangulator = triangulator;
		construire_cellules();
		return this->cellules;
	}

public:
	vector<Sommet<Vecteur2D>*>* germes;
private:
	Graphe<T, Vecteur2D>* graphe;
	Triangulator<S, T>* triangulator;
	vector<Triangle<S, T>*>* triangulation;
	vector<Face<S, T>*>* cellules;


	void construire_cellules() {
		bool fait[MAX_ARRAY] = { false };
		// Pour éviter les duplications 
		vector<ArcTU<T, S>*>* arcs_crees = new vector<ArcTU<T, S>*>;
		vector<Sommet<Vecteur2D>*>* sommets_crees = new vector<Sommet<Vecteur2D>*>;
		for (Triangle<S, T>* triangle : (*triangulation)) {
			for (ArcTU<T, S>* arc : triangle->arcs) {
				//Germe qu'on traite
				Sommet<Vecteur2D>* germe = arc->fin();
				// On vérifie si on a déjà traité ce germe
				if (!fait[germe->clef]) {
					germes->push_back(germe);
					pivoter_sur_germe(arc, triangle, sommets_crees, arcs_crees);
					fait[germe->clef] = true;
				}
			}
		}

	}

	/**
	* Créé un polygone autour de ce germe si c'est possible
	*/
	void pivoter_sur_germe(ArcTU<T, S>* arc, Triangle<S, T>* triangle, vector<Sommet<Vecteur2D>*>* sommets_crees, vector<ArcTU<T, S>*>* arcs_crees) {
		//Germe qu'on traite
		Sommet<Vecteur2D>* germe = arc->fin();
		//Cellules du diagramme de Voronoï
		vector<Sommet<Vecteur2D>*>* sommets_cellule = new vector<Sommet<Vecteur2D>*>;
		// Sommet de départ
		Triangle<S, T>* triangle_traite = triangle;
		
		while (true) {
			// On ajoute le centre à la liste des sommets de la face
			ajouter_sommet_cellule(triangle_traite, germe, sommets_cellule, sommets_crees);

			// On arrête quand on a bouclé
			if (sommets_cellule->size() > 2 && triangle_traite == triangle)
				break;

			if (sommets_cellule->size() > 2
				&& (sommets_cellule->back() == sommets_cellule->front() || sommets_cellule->at(sommets_cellule->size() - 2) == sommets_cellule->front()))
				break;


			if (trouver_triangle_adjacent(arc) == NULL)
				triangle_traite = traiter_cellule_infinie(triangle, triangle_traite, arc, germe, sommets_cellule, sommets_crees);
			else
				triangle_traite = trouver_triangle_adjacent(arc);

			// On décale à l'arc qui pointe vers le germe
			for (ArcTU<T, S>* a : triangle_traite->arcs)
				if (a->fin() == germe)
					arc = a;

		}
		creer_face(sommets_cellule, arcs_crees);
	}

	/**
	*	Ajoute les sommets de la cellule de Voronoï générée par germe sur ce triangle
	*/
	void ajouter_sommet_cellule(Triangle<S, T>* triangle, Sommet<Vecteur2D>* germe, vector<Sommet<Vecteur2D>*>* sommets_cellule, vector<Sommet<Vecteur2D>*>* sommets_crees) {
		Sommet<Vecteur2D>* centre_triangle = creer_sommet(triangle->cercle_circonscrit().centre, sommets_crees);
		double t, s;

		if(this->triangulator->contour->contientPointConcave(centre_triangle))
			ajouter_sommet(centre_triangle, sommets_cellule);
		else {
			for (ArcTU<T, S>* a : triangle->arcs) {
				// Cellule infinie, on ajoute qu'un seul sommet
				// Debut de la demie cellule
				if ((a->debut() == germe) && trouver_triangle_adjacent(a) == NULL) {
					for (ArcTU<T, S>* a2 : triangle->arcs) {
						if (a2->fin() == germe) {
							Vecteur2D centre_arc = (a2->debut()->v + a2->fin()->v) / 2;
							Vecteur2D inter = Geometrie::intersection(a->debut()->v, a->fin()->v, centre_arc, centre_triangle->v);
							ajouter_sommet(creer_sommet(inter, sommets_crees), sommets_cellule);
							return;
						}
					}
				}
				// Fin de la  demie-cellule
				else if ((a->fin() == germe) && trouver_triangle_adjacent(a) == NULL) {
					for (ArcTU<T, S>* a2 : triangle->arcs) {
						if (a2->debut() == germe) {
							Vecteur2D centre_arc = (a2->debut()->v + a2->fin()->v) / 2;
							Vecteur2D inter = Geometrie::intersection(a->debut()->v, a->fin()->v, centre_arc, centre_triangle->v);
							ajouter_sommet(creer_sommet(inter, sommets_crees), sommets_cellule);
							return;
						}
					}
				}
			}

			// Cellule complète, on ajoute les deux intersections sur la bordure
			// Premmière intersection
			for (ArcTU<T, S>* a : triangle->arcs)
				if (a->debut() == germe)
					ajouter_intersection(a, centre_triangle, triangle->arc_oppose(germe), sommets_cellule, sommets_crees);

			// Deuxième intersection
			for (ArcTU<T, S>* a : triangle->arcs)
				if (a->fin() == germe) 
					ajouter_intersection(a, centre_triangle, triangle->arc_oppose(germe), sommets_cellule, sommets_crees);
		}

	}

	/**
	*	Traite un cas d'exception:
	*	On essaye de pivoter sur un germe mais on tombe sur un arc qui n'a pas de triangle adjacent
	*	Si il n'y a pas de triangle adjacent, on ne peut pas créer de polygone complet, c'est une cellule infinie
	*/
	Triangle<S, T>* traiter_cellule_infinie(Triangle<S, T>* triangle_depart, Triangle<S, T>* triangle_traite, ArcTU<T, S>* arc, Sommet<Vecteur2D>* germe, vector<Sommet<Vecteur2D>*>* sommets_cellule, vector<Sommet<Vecteur2D>*>* sommets_crees) {
		double t, s;
		// On ajoute le centre si il n'est pas dehors, sinon on ajoute l'intersection
		for (ArcTU<T, S>* a : triangle_traite->arcs)
			if (a->debut() == germe) {
				Sommet<Vecteur2D>* centre_triangle = creer_sommet(triangle_traite->cercle_circonscrit().centre, sommets_crees);
				Vecteur2D centre_arc = (a->debut()->v + a->fin()->v) / 2;
				if (!Geometrie::intersectionSegmentSegment(arc->debut()->v, arc->fin()->v, centre_arc, centre_triangle->v, t, s)) {
					Sommet<Vecteur2D>* milieu_arc = creer_sommet(Vecteur2D((arc->debut()->v + arc->fin()->v) / 2), sommets_crees);
					ajouter_sommet(milieu_arc, sommets_cellule);
				}
			}

		// On ajoute le germe
		ajouter_sommet(germe, sommets_cellule);

		// On se place sur le triangle à l'autre extremité
		Triangle<S, T>* triangle_adjacent = triangle_depart;
		triangle_traite = triangle_depart;
		while (triangle_adjacent != NULL) {
			// On parcourt les triangles adjacents dans le sens horaire jusqu'à arriver à l'autre triangle extrême
			for (ArcTU<T, S>* a : triangle_adjacent->arcs) {
				if (a->debut() == germe) {
					triangle_adjacent = trouver_triangle_adjacent(a);
					if (triangle_adjacent != NULL)
						triangle_traite = triangle_adjacent;
					break;
				}
			}
		}

		ArcTU<T, S>* arc_sortant = NULL;
		ArcTU<T, S>* arc_entrant = NULL;
		// On cherche l'arc sortant et l'arc entrant du germe
		for (ArcTU<T, S>* a : triangle_traite->arcs) {
			if (a->debut() == germe)
				arc_sortant = a;
			else if (a->fin() == germe)
				arc_entrant = a;
		}

		// On ajoute l'intersection entre l'arc au bord et le centre du triangle traité
		Sommet<Vecteur2D>*  centre_triangle = creer_sommet(triangle_traite->cercle_circonscrit().centre, sommets_crees);
		Vecteur2D centre_arc = (arc_entrant->debut()->v + arc_entrant->fin()->v) / 2;

		if (!Geometrie::intersectionSegmentSegment(arc_sortant->debut()->v, arc_sortant->fin()->v, centre_arc, centre_triangle->v, t, s)) {
			Sommet<Vecteur2D>* milieu_arc = creer_sommet(Vecteur2D((arc_sortant->debut()->v + arc_sortant->fin()->v) / 2), sommets_crees);
			ajouter_sommet(milieu_arc, sommets_cellule);
		}

		return triangle_traite;
	}

	/**
	*	Ajoute s à sommets en veillant à ce que l'ordre soit respecté
	*/
	void ajouter_sommet(Sommet<Vecteur2D>* s, vector<Sommet<Vecteur2D>*>* sommets) {
		if (sommets->size() > 1 &&
			(sommets->at(sommets->size() - 2) == s || (sommets->back() == s)))
			return;
		if (sommets->size() > 0 && (sommets->back() == s))
			return;
		

		sommets->push_back(s);
	}

	/**
	*	Ajoute l'intersection entre le segment [milieu de l'arc, centre du triangle] et le contour à la liste des sommets de la cellule
	*/
	bool ajouter_intersection(ArcTU<T, S>* arc, Sommet<Vecteur2D>* centre_triangle, ArcTU<T, S>* arc_intersecte, vector< Sommet<Vecteur2D>*>* sommets_cellule, vector< Sommet<Vecteur2D>*>* sommets_crees) {
		double t, s;
		bool fait = false;
		Vecteur2D centre_arc = (arc->debut()->v + arc->fin()->v) / 2;

		for (ArcTU<T, S>* a : triangulator->contour->arcs)
			if (Geometrie::intersectionSegmentSegment(a->debut()->v, a->fin()->v, centre_arc, centre_triangle->v, t, s)) {
				Vecteur2D inter = Geometrie::intersection(a->debut()->v, a->fin()->v, centre_arc, centre_triangle->v);
				ajouter_sommet(creer_sommet(inter, sommets_crees), sommets_cellule);
				fait = true;
				break;
			}

		if (!fait) {
			ajouter_sommet(centre_triangle, sommets_cellule);
		}
			

		return fait;
	}

	/*bool ajouter_intersection(ArcTU<T, S>* arc, Sommet<Vecteur2D>* centre_triangle, ArcTU<T, S>* arc_intersecte, vector< Sommet<Vecteur2D>*>* sommets_cellule, vector< Sommet<Vecteur2D>*>* sommets_crees) {
		double t, s;
		bool fait = false;
		Vecteur2D centre_arc = (arc->debut()->v + arc->fin()->v) / 2;

		if (Geometrie::intersectionSegmentSegment(arc_intersecte->debut()->v, arc_intersecte->fin()->v, centre_arc, centre_triangle->v, t, s)) {
			Vecteur2D inter = Geometrie::intersection(arc_intersecte->debut()->v, arc_intersecte->fin()->v, centre_arc, centre_triangle->v);
			ajouter_sommet(creer_sommet(inter, sommets_crees), sommets_cellule);
			fait = true;
		}

		if (!fait) {
			//ajouter_sommet(centre_triangle, sommets_cellule);
			for (ArcTU<T, S>* a : triangulator->contour->arcs)
				if (Geometrie::intersectionSegmentSegment(a->debut()->v, a->fin()->v, centre_arc, centre_triangle->v, t, s)) {
					Vecteur2D inter = Geometrie::intersection(a->debut()->v, a->fin()->v, centre_arc, centre_triangle->v);
					ajouter_sommet(creer_sommet(inter, sommets_crees), sommets_cellule);
					fait = true;
					break;
				}
		}


		return fait;
	}*/

	/**
	* Crée un sommet en veillant à ce qu'il ne soit pas dupliqué
	*/
	Sommet<Vecteur2D>* creer_sommet(Vecteur2D sommet, vector<Sommet<Vecteur2D>*>* sommets_crees) {
		for (Sommet<Vecteur2D>* s : (*sommets_crees))
			if (s->v.x == sommet.x && s->v.y == sommet.y)
				return s;

		Sommet<Vecteur2D>* res = graphe->creeSommet(sommet);
		sommets_crees->push_back(res);
		return res;
	}

	/**
	* Crée un arc en veillant à ce qu'il ne soit pas dupliqué
	*/
	ArcTU<T, S>* creer_arc(Sommet<Vecteur2D>* deb, Sommet<Vecteur2D>* fin, Face<S, T>* face, vector<ArcTU<T, S>*>* arcs_crees) {
		for (ArcTU<T, S>* arc : (*arcs_crees))
			if (arc->arete->estEgal(deb, fin)) {
				ArcTU<T, S>* nouvel_arc = new ArcTU<T, S>(arc->arete, !arc->bonSens);
				nouvel_arc->face_adjacente = arc->face;
				nouvel_arc->arc_adjacent = arc;
				nouvel_arc->face = face;

				arc->face_adjacente = face;
				arc->arc_adjacent = nouvel_arc;
				return nouvel_arc;
			}

		ArcTU<T, S>* res = new ArcTU<T, S>(graphe->creeArete(T(), deb, fin), true);
		res->face = face;
		arcs_crees->push_back(res);
		return res;
	}

	void creer_face(vector<Sommet<Vecteur2D>*>* sommets_cellule, vector<ArcTU<T, S>*>* arcs_crees) {
		vector<ArcTU<T, S>*> arcs_cellule;
		Face<S, T>* face = new Face<S, T>();
		supprimer_sommets_doubles(sommets_cellule);

		for (int i = 0; i < sommets_cellule->size() - 1; i++) {
			ArcTU<T, S>* nouvel_arc = creer_arc(sommets_cellule->at(i), sommets_cellule->at(i + 1), face, arcs_crees);
			arcs_cellule.push_back(nouvel_arc);
		}
		face->arcs = arcs_cellule;
		this->cellules->push_back(face);
	}

	void supprimer_sommets_doubles(vector<Sommet<Vecteur2D>*>* sommets_cellule){
	retry:;
		for (int i = 0; i < sommets_cellule->size(); i++) {
			for (int j = 0; j < i; j++) {
				if (sommets_cellule->at(i) == sommets_cellule->at(j)) {
					sommets_cellule->erase(sommets_cellule->begin() + i);
					goto retry;
				}
			}
		}
		sommets_cellule->push_back(sommets_cellule->front());
	}


	/**
	* Retourne un triangle adjacent l'arc s'il en existe un, sinon retourne null
	*/
	Triangle<S, T>* trouver_triangle_adjacent(ArcTU<T, S>* arc) {
		return (Triangle<S, T>*)arc->face_adjacente;
	}

};
