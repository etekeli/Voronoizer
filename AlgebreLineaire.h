/*
 * AlgebreLineaire.h
 *
 *  Created on: 30 juin 2011
 *      Author: Dominique
 */

#ifndef ALGEBRELINEAIRE_H_
#define ALGEBRELINEAIRE_H_

#include <math.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

template <class T>
inline const T operator - (const T& u, const T& v)
{
	return u + -v;
}

template <class T>
inline const T operator / (const T& u, const double& a)
{
	return u * (1 / a);
}

template <class T>
inline const T& operator *=(T& u, const T& v)
{
	u = u * v;

	return u;
}

template <class T>
inline const T& operator ^=(T& u, const T& v)
{
	u = u ^ v;

	return u;
}

template <class T>
inline const T& operator -= (T& u, const T& v)
{
	return u += -v;
}

template <class T>
inline const T& operator /= (T& u, const double& a)
{
	return u *= (1 / a);
}

template <class T>
inline double norme(const T& u)
{
	return sqrt(u * u);
}

template <class T>
inline double cos(const T& u1, const T& u2) { return (u1 * u2) / (norme(u1) * norme(u2)); }


template <class T>
inline double norme2(const T& u)
{
	return (u * u);
}

template <class T>
inline bool operator != (const T& u, const T& v)
{
	return !(u == v);
}

/*
 * D�finit un couple de valeurs o� les 2 valeurs ne sont pas forc�ment de m�me nature
 *
 * */
template < class S, class T>
class Paire
{
public:
	S x; T y;
	Paire(const S& x = (S)0, const T& y = (T)-1) : x(x), y(y) {}

	operator string () const { ostringstream o; o << "( " << x << ", " << y << ")"; return o.str(); }

	friend ostream& operator << (ostream& o, const Paire<S, T>& paire) { return o << (string)paire; }

};

#endif /* ALGEBRELINEAIRE_H_ */
