#pragma once
#include "Arete.h"
#include "Sommet.h"

template <class S, class T>
class Face;

/**
* S est la nature de l'information portée par une arête
* T est la nature de l'information portée par une face
*/
template <class S, class T>
class ArcTU {
public:
	Arete<S, Vecteur2D> *arete;
	bool bonSens; // vrai si dans le bon sens
	Face<T, S>* face = NULL; // face à laquelle cet arc appartient
	Face<T, S>* face_adjacente = NULL; // face adjacente à cet arc
	ArcTU<S, T>* arc_adjacent = NULL; //arc adjacent à cet arc

	ArcTU(Arete<S, Vecteur2D>* arete, int bonSens): arete(arete), bonSens(bonSens){
		arete->degre++;
	}

	~ArcTU() {
		arete->degre--;
		if (arc_adjacent != NULL) {
			arc_adjacent->arc_adjacent = NULL;
			arc_adjacent->face_adjacente = NULL;
		}
	}

	/**
	* Retourne vrai si le sommet s est à gauche de cet arc (ou s'il est collinéaire)
	*/
	bool estAGauche(Sommet<Vecteur2D> *s) {
		Vecteur2D a = debut()->v;
		Vecteur2D b = fin()->v;

		return (b.x - a.x) * (s->v.y - a.y) - (s->v.x - a.x) * (b.y - a.y) > 0;
	}

	/**
	* Retourne vrai si le sommet s est à gauche de cet arc
	*/
	bool estCollineaire(Sommet<Vecteur2D>* s) {
		Vecteur2D a = debut()->v;
		Vecteur2D b = fin()->v;

		return (b.x - a.x) * (s->v.y - a.y) - (s->v.x - a.x) * (b.y - a.y) == 0;
	}

	/**
	* Retourne le début de l'arc
	*/
	Sommet<Vecteur2D>* debut() {
		if (this->bonSens)
			return arete->debut;
		else
			return arete->fin;
	}

	/**
	* Retourne la fin de l'arc
	*/
	Sommet<Vecteur2D>* fin() {
		if (this->bonSens)
			return arete->fin;
		else
			return arete->debut;
	}

	friend bool operator==(const ArcTU<S, T>& lhs, const ArcTU<S, T>& rhs) {
		return (lhs.arete->estEgal(rhs.arete->debut, rhs.arete->fin) && lhs.bonSens == rhs.bonSens);
	}

};