#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "AElement.h"

using namespace std;

/**

Représente la classe template de base des éléments d'un graphe qui peuvent être des sommets ou des arêtes.

A ce niveau, on ne peut que définir que la clef d'un élément et la valeur (c'est-à-dire l'information ) inscrite dans cet élément.

En effet tout élément d'un graphe est défini par une clef (c-à-d une valeur non nulle et unique) et contient  généralement une information,
qui peut être un nom, une couleur, une position, etc.



*/

template <class T>
class GElement : public AElement
{
public:
	T v;
	GElement(const int clef, const T& v) : AElement(clef), v(v) {}
	operator string() const { ostringstream o; o << this->AElement::operator string() << ", v = " << v; return o.str(); }

	friend ostream& operator << (ostream& os, const GElement<T>& gElement) { return os << (string)gElement; }
};


