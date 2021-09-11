#pragma once
#include "Vecteur2D.h"
#include "Erreur.h"

class Geometrie {
public:
	Geometrie(){}
	~Geometrie(){}

	/**
	* Retourne vrai si c est à gauche de (a,b) ou 
	*/
	static bool aGauche(Vecteur2D a, Vecteur2D b, Vecteur2D c) {
		return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y) > 0;
	}

	/**
	* Retourne vrai si c est à gauche de (a,b) ou s'ils sont collinéaires
	*/
	static bool aGaucheOuCollineaire(Vecteur2D a, Vecteur2D b, Vecteur2D c) {
		return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y) >= 0;
	}

	/**
	* Retourne vrai si a b c est collinéaire
	*/
	static bool collineaire(Vecteur2D a, Vecteur2D b, Vecteur2D c) {
		return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y) == 0;
	}

	/**
	* Retourne vrai si abcd est un poylgone convexe
	*/
	static bool forment_polygone_convexe(Vecteur2D a, Vecteur2D b, Vecteur2D c, Vecteur2D d) {
		bool res = false;

		bool abc = aGauche(a, b, c);
		bool abd = aGauche(a, b, d);
		bool bcd = aGauche(b, c, d);
		bool cad = aGauche(c, a, d);

		if (abc && abd && bcd && !cad)
			res = true;
		else if (abc && abd && !bcd && cad)
			res = true;
		else if (abc && !abd && bcd && cad)
			res = true;
		else if (!abc && !abd && !bcd && cad)
			res = true;
		else if (!abc && !abd && bcd && !cad)
			res = true;
		else if (!abc && abd && !bcd && !cad)
			res = true;

		return res;
	}

	static double det(Vecteur2D a, Vecteur2D b) {
		return a.x *b.y - a.y *b.x;
	}

	static double det(double a1, double a2, double b1, double b2) {
		return a1 * b2 - b1 * a2;
	}

	/**
	 * a1 * t + b1 * s = c1
	 * a2 * t + b2 * s = c2
	 * 
	 */
	static void resoudreSysteme(const double a1, const double b1, const double c1, const double a2, const double b2, const double c2, double& t, double &s) {

		double ddet = det(a1, a2, b1, b2);

		t = det(a1, a2, c1, c2) / ddet;
		s = det(c1, c2, b1, b2) / ddet;
	}

	/**
	 */
	static void intersectionDroiteDroite(const Vecteur2D &p0, const Vecteur2D &p1, const Vecteur2D &q0, const Vecteur2D &q1,
							double &t, double &s){

		Vecteur2D q0p0 = p0 - q0;
		Vecteur2D q0q1 = q1 - q0;
		Vecteur2D p0p1 = (p1 - p0) * -1;

		Geometrie::resoudreSysteme(p0p1.x, q0q1.x, q0p0.x, p0p1.y, q0q1.y, q0p0.y, t, s);
	}


	static bool intersectionSegmentSegment(const Vecteur2D& p0, const Vecteur2D& p1, const Vecteur2D& q0, const Vecteur2D& q1,
		double& t, double& s) {
		intersectionDroiteDroite(p0, p1, q0, q1, t, s);
		return (0 < t && t < 1 && 0 < s && s < 1);
	}

	static bool intersection(const Vecteur2D& p0, const Vecteur2D& p1, const Vecteur2D& q0, const Vecteur2D& q1, Vecteur2D& intersection) {

		double s, t;
		intersectionDroiteDroite(p0, p1, q0, q1, s, t);

		Vecteur2D p0M = (p1 - p0) * t;
		Vecteur2D q0M = (q1 - q0) * s;

		intersection = p0 + p0M;
		return (0 < t && t < 1 && 0 < s && s < 1);
	}

	static Vecteur2D intersection(const Vecteur2D& p0, const Vecteur2D& p1, const Vecteur2D& q0, const Vecteur2D& q1) {

		Vecteur2D inter;
		intersection(p0, p1, q0, q1, inter);
		return inter;
	}

};

