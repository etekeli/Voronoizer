#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>

#include "Sommet.h"
#include "Triangle.h"
#include "Graphe.h"
#include "Cercle.h"

#define MAX 10000

using namespace std;

/*
* Classe Triangulator
* Prends en entre une liste de sommets
* et crée une triangulation de Delaunay
*
* T est la nature de l'information porte par une arête
* S est la nature de l'information porte par une face
*/
template <class S, class T>
class Triangulator {
public:
	Triangulator() { }
	~Triangulator() {}

	/**
	* Retourne une triangulation de Delaunay
	*/
	vector<Triangle<S, T>*>* triangulate(vector<Sommet<Vecteur2D>*>* sommets, Graphe<T, Vecteur2D>* graphe) {
		init(sommets, graphe);
		determiner_triangulation_englobante();
		for (Sommet<Vecteur2D>* s : (*sommets)) {
			Triangle<S, T>* t = determiner_triangle_contenant_sommet(s);
			if (t) {
				determiner_DTL(s, t);
				determiner_NTL(s);
				supprimer_DTL();
			}
			else 
				throw Erreur("Aucun triangle contenant le sommet trouvé");
		}
		supprimer_englobants();
		calculer_contour();

		return triangulation;
	}

	Face<S, T>* calculer_contour() {
		// On cherche les triangles au bord
		vector<Triangle<S, T>*>* triangles_au_bord = new vector<Triangle<S, T>*>;
		for (Triangle<S, T>* t : (*triangulation))
			for (ArcTU<T, S>* arc : t->arcs)
				if (trouver_triangle_adjacent(arc) == NULL) {
					triangles_au_bord->push_back(t);
					break;
				}

		// Puis on ajoute dans le bon ordre les arcs au bord
		vector<ArcTU<T, S>*> arcs;
		for (ArcTU<T, S>* arc : triangles_au_bord->front()->arcs)
			if (trouver_triangle_adjacent(arc) == NULL)
				arcs.push_back(arc);

		triangles_au_bord->erase(triangles_au_bord->begin());
		while (triangles_au_bord->size()) {
			int i = 0;
			for (Triangle<S, T>* t : (*triangles_au_bord)) {
				for (ArcTU<T, S>* arc : t->arcs) {
					if (arcs.back()->fin() == arc->debut()) {
						arcs.push_back(arc);
						triangles_au_bord->erase(triangles_au_bord->begin() + i);
						goto next;
					}
				}
				i++;
			}
		next:;
		}

		this->contour = new Face<S, T>(arcs, S());
		return contour;
	}

public:
	double left, right, bottom, top;
	Face<S, T>* contour;
private:
	vector<Sommet<Vecteur2D>*>* sommets; //Sommets en entrée, les sommets ne doivent pas être dupliqués
	Graphe<T, Vecteur2D>* graphe; //Graphe utilisé pour créer des sommets
	vector<Triangle<S, T>*>* triangulation;//Triangles en sortie
	vector<Triangle<S, T>*>* DTL; //Triangles à supprimer de la triangulation
	Sommet<Vecteur2D>* sommetsEnglobants[4] = { NULL }; //Sommets englobants les autres sommets, à supprimer à la fin



	/**
	* Initialise les membres
	*/
	void init(vector<Sommet<Vecteur2D>*>* sommets, Graphe<T, Vecteur2D>* graphe) {
		this->sommets = sommets;
		this->graphe = graphe;
		triangulation = new vector<Triangle<S, T>*>();
		DTL = new vector<Triangle<S, T>*>();
	}

	/**
	* Détermine une triangulation qui englobe tous les sommets à trianguler
	*/
	void determiner_triangulation_englobante() {
		/* On cherche les points extrêmes */
		left = sommets->front()->v.x;
		right = sommets->front()->v.x;
		bottom = sommets->front()->v.y;
		top = sommets->front()->v.y;

		for (Sommet<Vecteur2D>* s : (*sommets)) {
			if (s->v.x < left) left = s->v.x;
			if (s->v.x > right) right = s->v.x;
			if (s->v.y < bottom) bottom = s->v.y;
			if (s->v.y > top) top = s->v.y;
		}

		/* On crée les sommets/artes d'un rectangle avec ces points */
		Sommet<Vecteur2D>* s0, * s1, * s2, * s3;
		double marge = 10; //pour éviter les sommets superposés
		s0 = graphe->creeSommet(Vecteur2D(left - marge, bottom - marge));
		s1 = graphe->creeSommet(Vecteur2D(right + marge, bottom - marge));
		s2 = graphe->creeSommet(Vecteur2D(right + marge, top + marge));
		s3 = graphe->creeSommet(Vecteur2D(left - marge, top + marge));

		sommetsEnglobants[0] = s0;
		sommetsEnglobants[1] = s1;
		sommetsEnglobants[2] = s2;
		sommetsEnglobants[3] = s3;

		/*
		s3		 a1			s2
			-------------
		a2	|	a0	-	|	a4
			|	-		|
		s0	-------------	s1
					a3
		*/

		/* On crée une triangulation de ce rectangle */
		Triangle<S, T>* t1 = new Triangle<S, T>();
		Triangle<S, T>* t2 = new Triangle<S, T>();
		vector<ArcTU<T, S>*>* arcs_crees = new vector<ArcTU<T, S>*>;
		vector<ArcTU<T, S>*> arcs1;
		vector<ArcTU<T, S>*> arcs2;

		arcs1.push_back(creer_arc(s0, s2, t1, arcs_crees));
		arcs1.push_back(creer_arc(s2, s3, t1, arcs_crees));
		arcs1.push_back(creer_arc(s3, s0, t1, arcs_crees));
		t1->arcs = arcs1;

		arcs2.push_back(creer_arc(s0, s2, t2, arcs_crees));
		arcs2.push_back(creer_arc(s0, s1, t2, arcs_crees));
		arcs2.push_back(creer_arc(s1, s2, t2, arcs_crees));
		t2->arcs = arcs2;

		triangulation->push_back(t1);
		triangulation->push_back(t2);
	}

	/**
	* Retourne le triangle contenant le sommet s, retourn null s'il n'en existe pas
	*/
	Triangle<S, T>* determiner_triangle_contenant_sommet(Sommet<Vecteur2D>* s) {
		for (Triangle<S, T>* t : (*triangulation))
			if (t->contientPoint(s))
				return t;

		return NULL;
	}

	// Détermine la liste des triangles  supprimer de la triangulation
	void determiner_DTL(Sommet<Vecteur2D>* s, Triangle<S, T>* t) {
		if (t != NULL) {
			DTL->push_back(t);
			for (int i = 0; i < 3; i++) {
				Triangle<S, T>* triangleAdjacent = trouver_triangle_adjacent(t->arcs[i]);
				if (triangleAdjacent != NULL && count(DTL->begin(), DTL->end(), triangleAdjacent) == 0) {
					Cercle cercle = triangleAdjacent->cercle_circonscrit();
					if (cercle.contientPoint(s->v))
						determiner_DTL(s, triangleAdjacent);
				}
			}
		}
	}

	/**
	* Détermine la liste des triangles à rajouter à la triangulation
	*/
	void determiner_NTL(Sommet<Vecteur2D>* s) {
		vector<ArcTU<T, S>*>* arcs_crees = new vector<ArcTU<T, S>*>;
		for (Triangle<S, T>* t : (*DTL)) {
			for (int i = 0; i < 3; i++) {
				Triangle<S, T>* triangleAdjacent = trouver_triangle_adjacent(t->arcs[i]);
				if ((triangleAdjacent == NULL || count(DTL->begin(), DTL->end(), triangleAdjacent) == 0)
					&& !t->arcs[i]->estCollineaire(s)) {
					creer_triangle(t->arcs[i], s, arcs_crees);
				}
			}
		}
	}

	/**
	* Crée un triangle avec l'arc guide et le sommet s
	* utilise les arcs déjà créés pour ne pas dupliquer les arêtes et garder la convention du bonhomme d'ampère
	*/
	void creer_triangle(ArcTU<T, S>* arc_guide, Sommet<Vecteur2D>* s, vector<ArcTU<T, S>*>* arcs_crees) {
		//On crée le nouveau triangle
		vector<ArcTU<T, S>*> arcs;
		Triangle<S, T>* triangle = new Triangle<S, T>();

		//premier arc, on supprime l'arc guide et on rétablit l'adjacence en créant une nouvel arc

		ArcTU<T, S>* arc_adjacent = arc_guide->arc_adjacent;
		ArcTU<T, S>* nouvel_arc = new ArcTU<T, S>(arc_guide->arete, arc_guide->bonSens);
		arc_guide->arc_adjacent = NULL;// on détache les arcs pour éviter les problèmes après suppression de face 
//		arc_guide->ar
		nouvel_arc->arc_adjacent = arc_adjacent;
		nouvel_arc->face = triangle;
		nouvel_arc->face_adjacente = arc_guide->face_adjacente;

		if (arc_adjacent != NULL) {
			arc_adjacent->face_adjacente = triangle;
			arc_adjacent->arc_adjacent = nouvel_arc;
		}
		arcs.push_back(nouvel_arc);


		//deuxième arc, si on a créé un arc ayant la même arête, on réutilise l'arête
		arcs.push_back(creer_arc(arc_guide->fin(), s, triangle, arcs_crees));

		//troisième arc, même logique que pour le deuxième
		arcs.push_back(creer_arc(s, arc_guide->debut(), triangle, arcs_crees));

		//on l'ajoute à la triangulation
		triangle->arcs = arcs;
		this->triangulation->push_back(triangle);
	}

	/**
	* Retourne un triangle adjacent l'arc s'il en existe un, sinon retourne null
	*/
	Triangle<S, T>* trouver_triangle_adjacent(ArcTU<T, S>* arc) {
		return (Triangle<S, T>*) arc->face_adjacente;
	}

	/**
	* Crée un arc en veillant à ce qu'il ne soit pas dupliqué
	*/
	ArcTU<T, S>* creer_arc(Sommet<Vecteur2D>* deb, Sommet<Vecteur2D>* fin, Triangle<S, T> * triangle, vector<ArcTU<T, S>*>* arcs_crees) {
		for (ArcTU<T, S>* arc : (*arcs_crees))
			if (arc->arete->estEgal(deb, fin)) {
				ArcTU<T, S>* nouvel_arc = new ArcTU<T, S>(arc->arete, !arc->bonSens);
				nouvel_arc->face_adjacente = arc->face;
				nouvel_arc->arc_adjacent = arc;
				nouvel_arc->face = triangle;

				arc->face_adjacente = triangle;
				arc->arc_adjacent = nouvel_arc;
				return nouvel_arc;
			}

		ArcTU<T, S>* res = new ArcTU<T, S>(graphe->creeArete(T(), deb, fin), true);
		res->face = triangle;
		arcs_crees->push_back(res);
		return res;
	}

	/**
	* Supprime les triangulation en commun dans DTL et Triangles
	*/
	void supprimer_DTL() {
		int i;
		for (Triangle<S, T>* dt : (*DTL)) {
			i = 0;
			for (Triangle<S, T>* t : (*triangulation)) {
				if (dt == t) {
					triangulation->erase((triangulation->begin() + i));
					break;
				}
				i++;
			}
		}

		for (auto& t : (*DTL)) {
			for (ArcTU<T, S>* arc : t->arcs)
				if (arc->arete->degre < 2)
					graphe->lAretes->retire(arc->arete, graphe->lAretes);
			
			delete t;
			t = nullptr;
		}
		DTL->erase(remove(DTL->begin(), DTL->end(), nullptr), DTL->end());
	}


	/**
	* Supprime les sommets englobants élague les arêtes arrivant à ce sommet
	*/
	void supprimer_englobants() {
		for (auto it = triangulation->begin(); it != triangulation->end(); ) {
			bool deleted = false;
			Triangle<S, T>* t = (Triangle<S, T>*) * it;
			for (ArcTU<T, S>* arc : (t->arcs)) {
				for (int j = 0; j < 4; j++) {
					if (arc->debut() == sommetsEnglobants[j] || arc->fin() == sommetsEnglobants[j]) {
						delete* it;
						it = triangulation->erase(it);
						deleted = true;
						goto next;
					}
				}
			}
		next:
			if (!deleted)
				++it;
		}
	}

};
