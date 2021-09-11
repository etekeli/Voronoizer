#pragma once

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

/**

Repr�sente la classe de base des �l�ments d'un graphe qui peuvent �tre des sommets ou des ar�tes.

A ce niveau, on ne peut que d�finir que la clef d'un �l�ment inscrite dans cet �l�ment.

En effet tout �l�ment d'un graphe est tout d'abord d�fini par une clef (c-�-d une valeur non nulle et unique)

*/

class AElement
{
public:
	int clef;

	explicit AElement(const int clef) : clef(clef) {}
	operator string() const { ostringstream o; o << "clef = " << clef; return o.str(); }

	friend ostream& operator << (ostream& os, const AElement& aElement) { return os << (string)aElement; }
};
