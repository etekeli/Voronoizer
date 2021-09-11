/**
 * Vecteurs2D.h
 *
 *  Created on: 1 juil. 2011
 *      Author: Valérie
 */


#ifndef VECTEUR2D_H_
#define VECTEUR2D_H_

#include <iostream>
#include <string>
#include <cmath>
#include "AlgebreLineaire.h"
#include <float.h>
#include "Reels.h"


using namespace std;

class Vecteur2D
{
public:
	double x, y;

	inline explicit Vecteur2D(const double& x = 0, const double& y = 0);

	static const Vecteur2D creeVecteur2D(const double& x, const double& y) { return Vecteur2D(x, y); }


	/**
	 *
	 * RESULTAT : un vecteur aléatoire x tel que | x - germe | < rayon
	 *
	 * suppose qu'un appel à srand a été fait au préalable
	 *
	 * */
	inline static const Vecteur2D random(const Vecteur2D& germe, const double& rayon);


	/**
	 * DONNEES : s respectant le format "(  nombre réel, nombre réel)"
	 *
	 * */
	 //inline Vecteur2D(const char * s);
	inline const Vecteur2D operator + (const Vecteur2D& u) const;
	inline const Vecteur2D operator * (const double& a) const;
	/**
	 * multiplication au sens des nombres complexes
	 * */
	inline const Vecteur2D operator ^ (const Vecteur2D& u) const;
	inline const Vecteur2D operator - () const;
	/**
	 * produit scalaire
	 * */
	inline double operator * (const Vecteur2D& u) const;

	//static double produitScalaire (const Vecteur2D & u1, const Vecteur2D & u2) {return u1*u2;}


	const Vecteur2D rotationDirecteQuartDeTour() const { return Vecteur2D(-y, x); }


	/**
	 * crée le vecteur (y,x) à partir du vecteur (x,y)
	 * */
	inline const Vecteur2D swap() const { return Vecteur2D(y, x); }


	/**
	 * déterminant (u1,u2)
	 * */
	static inline double det(const Vecteur2D& u1, const Vecteur2D& u2);

	inline bool operator == (const Vecteur2D& u) const { return x == u.x && y == u.y; }

	inline const Vecteur2D& operator +=(const Vecteur2D& u);
	inline const Vecteur2D& operator *=(const double& a);

	/**
	 * DONNEE : *this
	 * RESULTATS : xArrondi, yArrondi
	 * TACHE : calcule les valeurs arrondies des coordonnées de *this
	 *
	 * */
	template <class T>
	void arrondit(T& xArrondi, T& yArrondi) const { xArrondi = (T)x; yArrondi = (T)y; }

	operator string() const { ostringstream os; os << "( " << x << ", " << y << ")"; return os.str(); }

	/**
	 * calcule |u|_sup, c-à-d max(|x|,|y|)
	 * */
	inline static double normeSup(const Vecteur2D& u) { return max(abs(u.x), abs(u.y)); }


	//------------------ Vecteur2D ----------------------------------
};

inline ostream& operator << (ostream& os, const Vecteur2D& u) { os << (string)u; return os; }

inline const Vecteur2D operator *(const double& a, const Vecteur2D& u) { return u * a; }

/**
 * calcule |u|_sup, c-à-d max(|x|,|y|)
 * */
inline double normeSup(const Vecteur2D& u) { return Vecteur2D::normeSup(u); }

//----------------------------- implémentation des fonctions inline ----------------------------------

inline  Vecteur2D::
Vecteur2D(const double& x, const double& y) : x(x), y(y) {}

/**
 * DONNEES : s respectant le format "(  nombre réel, nombre réel)"
 *
 * */
 /*inline*/ /*Vecteur2D::Vecteur2D(const char * s)
 {
 extrait(s,x,y);
 }*/

 /**
  *
  * RESULTAT : un vecteur aléatoire x tel que | x - germe | < rayon
  *
  * suppose qu'un appel à srand a été fait au préalable
  *
  * */
inline /*static*/ const Vecteur2D Vecteur2D::random(const Vecteur2D& germe, const double& rayon)
{
	double angle = Reels::random(0, 1) * Reels::PI; // -PI <= angle < PI
	double x = cos(angle), y = sin(angle);
	double r = Reels::random() * rayon;       // 0 <= r < rayon
	return germe + r * Vecteur2D(x, y);
}

inline const Vecteur2D random(const Vecteur2D& germe, const double& rayon) { return Vecteur2D::random(germe, rayon); }

inline const Vecteur2D Vecteur2D::operator + (const Vecteur2D& u) const
{
	return Vecteur2D(x + u.x, y + u.y);
}
inline const Vecteur2D Vecteur2D::operator * (const double& a) const
{
	return Vecteur2D(x * a, y * a);
}
/**
 * multiplication au sens des nombres complexes
 * */
inline const Vecteur2D Vecteur2D::operator ^ (const Vecteur2D& u) const
{
	return Vecteur2D(x * u.x - y * u.y, x * u.y + y * u.x);
}
inline const Vecteur2D Vecteur2D::operator - () const
{
	return Vecteur2D(-x, -y);
}
/**
 * produit scalaire
 * */
inline double Vecteur2D::operator * (const Vecteur2D& u) const
{
	return x * u.x + y * u.y;
}

/**
 *
 * déterminant(u1,u2)
 *
 * */
/*static*/ inline double Vecteur2D::det(const Vecteur2D& u1, const Vecteur2D& u2) { return u1.x * u2.y - u1.y * u2.x; }

inline const Vecteur2D& Vecteur2D::operator +=(const Vecteur2D& u)
{
	x += u.x;
	y += u.y;
	return *this;
}

inline const Vecteur2D& Vecteur2D::operator *=(const double& a)
{
	x *= a;
	y *= a;
	return *this;
}

/**
 * Calcule en rad dans l'intervalle ]-PI,PI] la mesure principale de l'angle orienté v1 -> v2
 *
 * Donne des résultats inexploitables si v1 = 0 ou si v2 = 0
 *
 * */
inline double angleOriente(const Vecteur2D& v1, const Vecteur2D& v2)
{
	double nv1v2 = norme(v1) * norme(v2);

	double sinv1v2 = Vecteur2D::det(v1, v2) / nv1v2;
	double cosv1v2 = (v1 * v2) / nv1v2;

	return atan2(sinv1v2, cosv1v2);

}






#endif /* VECTEUR2D_H_ */
