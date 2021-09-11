#pragma once

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

/**

Représente la classe de base des éléments d'un graphe qui peuvent être des sommets ou des arêtes.

A ce niveau, on ne peut que définir que la clef d'un élément inscrite dans cet élément.

En effet tout élément d'un graphe est tout d'abord défini par une clef (c-à-d une valeur non nulle et unique)

*/

class AElement
{
public:
	int clef;

	explicit AElement(const int clef) : clef(clef) {}
	operator string() const { ostringstream o; o << "clef = " << clef; return o.str(); }

	friend ostream& operator << (ostream& os, const AElement& aElement) { return os << (string)aElement; }
};
