#pragma once
#include "Vecteur2D.h"


class Cercle {
public:
	double rayon;
	Vecteur2D centre;

	Cercle(Vecteur2D centre, double rayon) : centre(centre), rayon(rayon) {}

	/**
	* Construit ce cercle circonscrit au triangle formé par les 3 sommets
	*/
	Cercle(Vecteur2D a, Vecteur2D b, Vecteur2D c){
		// Les formules : https://en.wikipedia.org/wiki/Circumscribed_circle#Circumcenter_coordinates
		double d = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));

		centre.x = ((a.x * a.x + a.y * a.y) * (b.y - c.y) + (b.x * b.x + b.y * b.y) * (c.y - a.y) + (c.x * c.x + c.y * c.y) * (a.y - b.y)) / d;
		centre.y = ((a.x * a.x + a.y * a.y) * (c.x - b.x) + (b.x * b.x + b.y * b.y) * (a.x - c.x) + (c.x * c.x + c.y * c.y) * (b.x - a.x)) / d;

		//Le rayon est égal à la distance entre le centre du cercle circonscrit et un des sommets du triangle
		rayon = sqrt((a.x - centre.x) * (a.x - centre.x) + (a.y - centre.y) * (a.y - centre.y));
	}

	/**
	* Retourne vrai si ce cercle contient le sommet s
	*/
	bool contientPoint(Vecteur2D s) {
		return (s.x - centre.x) * (s.x - centre.x) + (s.y - centre.y) * (s.y - centre.y) </*=*/ rayon * rayon;
	}
};