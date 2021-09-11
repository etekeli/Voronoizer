#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include "GElement.h"
#include "Sommet.h"

using namespace std;

/**

représente une arête d'un graphe en général.

Une arête est définie par un sommet-début et par un sommet-fin et par une information v.

On ne connaît pas la nature de v à ce niveau (v pourrait contenir une couleur, une longueur, un nom, etc.)

T est la nature de l'information portée par un sommet et
S est la nature de l'information portée par une arête

*/

template <class S, class T>
class Arete : public GElement<S>
{
public:
	Sommet <T>* debut, * fin;	// debut et fin peuvent être interprétées comme des clefs étrangères vers une table de sommets
	int degre = 0;

	/**
	met à jour le degré des sommets que cette nouvelle arête va connecter
	*/
	Arete(const int clef, const S& v, Sommet<T>* debut, Sommet<T>* fin) : GElement<S>(clef, v), debut(debut), fin(fin)
	{
		debut->degre++; fin->degre++;
	}

	/**
	met à jour le degré des sommets que cette nouvelle arête va déconnecter
	*/
	~Arete() { debut->degre--; fin->degre--; }

	operator string () const;


	/**
	 * vérifie si *this s'appuie sur s1 et s2
	 *
	 * DONNEES : *this, s1, s2
	 *
	 * RESULTATS : true si *this s'appuie sur s1 et s2 c'est-à-dire si (début == s1 et fin == s2) ou (début == s2 et fin == s1), false sinon
	 *
	 * */
	bool estEgal(const Sommet<T>* s1, const Sommet<T>* s2) const;
};

template <class S, class T>
Arete<S, T>::operator string () const
{
	ostringstream oss;

	oss << "Arete  (" << endl;
	oss << GElement<S>::operator string() << endl;
	oss << "clef debut = " << debut->clef << endl;
	oss << "clef fin = " << fin->clef << endl;
	oss << ")";
	return oss.str();

}

template <class S, class T>
ostream& operator << (ostream& os, const Arete<S, T>& arete)
{
	return os << (string)arete;
}

/**
 * vérifie si *this s'appuie sur s1 et s2
 *
 * DONNEES : *this, s1, s2
 *
 * RESULTATS : true si *this s'appuie sur s1 et s2 c'est-à-dire si (début == s1 et fin == s2) ou (début == s2 et fin == s1), false sinon
 *
 * */
template <class S, class T>
bool Arete<S, T>::estEgal(const Sommet<T>* s1, const Sommet<T>* s2) const
{
	return (s1 == debut && s2 == fin) || (s1 == fin && s2 == debut);
}
