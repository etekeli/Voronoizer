#pragma once
#include <iostream>
#include <ostream>
#include <string>
#include <sstream>

int n = 0;

template <class T>
class PElement {
public:
	PElement<T>* suivant;
	T* valeur;

	PElement(PElement<T>* s, T* v);
	static int taille(const PElement<T>* l);
	static const std::string toString(const PElement<T>* p,
		const char* debut = "( ",
		const char* separateur = ", ",
		const char* fin = ") ");
	static void insertionOrdonnee(T* a, PElement<T>*& l,
		bool (*estPlusPetitOuEgal)(const T* a1, const T* a2));
	static T* depiler(PElement<T>*& l);
	static bool retire(const T* a, PElement<T>*& l);
	static void efface1(PElement<T>*& l);
	static void efface2(PElement<T>*& l);
	static PElement<T>* copie1(PElement<T>* l);
	static PElement<T>* copie2(PElement<T>* l);
	static PElement<T>* copie3(PElement<T>* l);
	static PElement<T>* appartient(const T* a, PElement<T>* l);

	template< class FONCTEUR>
	static PElement<T>* appartient(PElement<T>* l, const FONCTEUR& condition);
};


/**     Implementation ****/
template <class T>
PElement<T>::PElement(PElement<T>* s, T* v) {
	suivant = s;
	valeur = v;
}

template <class T>
int PElement<T>::taille(const PElement<T>* l) {
	n++;
	std::cout << n << std::endl;
	if (l == nullptr)
		return 0;
	else
		return 1 + PElement<T>::taille(l->suivant);
}

template <class T>
const std::string PElement<T>::toString(const PElement<T>* p,
	const char* debut,
	const char* separateur,
	const char* fin)
{
	std::ostringstream os;
	os << debut;
	while (p != nullptr) {
		os << *(p->valeur) << separateur;
		p = p->suivant;
	}
	os << fin;
	return os.str();
}
template <class T>
std::ostream& operator <<(std::ostream& os, const PElement<T>* p) {
	return os << PElement<T>::toString(p);
}
template <class T>
void PElement<T>::insertionOrdonnee(T* a, PElement<T>*& l,
	bool (*estPlusPetitOuEgal)(const T* a1, const T* a2))
{
	if (l == nullptr)
		l = new PElement<T>(nullptr, a);
	else
		if (estPlusPetitOuEgal(a, l->valeur))
			l = new PElement<T>(l, a);
		else
			insertionOrdonnee(a, l->suivant, estPlusPetitOuEgal);
}
template <class T>
T* PElement<T>::depiler(PElement<T>*& l) {
	if (l == nullptr) throw ("Fatal error, Liste vide");
	T* tete = l->valeur;
	PElement<T>* tmp = l;
	l = l->suivant;
	delete tmp;
	return tete;
}
template<class T>
bool PElement<T>::retire(const T* a, PElement<T>*& l) {
	if (l == nullptr) {
		return false;
	}
	else {
		if (l->valeur == a) {
			PElement<T>* tmp = l;
			l = l->suivant;
			delete tmp;
			return true;
		}
		else return retire(a, l->suivant);
	}
}
template <class T>
void PElement<T>::efface1(PElement<T>*& l) {
	if (l != nullptr)
	{
		PElement<T>* tmp = l;
		l = l->suivant;
		delete tmp;
		efface1(l);
	}
}
template <class T>
void PElement<T>::efface2(PElement<T>*& l) {
	if (l != nullptr) {
		PElement<T>* tmp = l;
		l = l->suivant;
		delete tmp->valeur;
		delete tmp;
		efface2(l);
	}
}
template <class T>
PElement<T>* PElement<T>::copie1(PElement<T>* l) {
	if (l == nullptr) return nullptr;
	else return new PElement<T>(copie1(l->suivant), l->valeur);
}
template <class T>
PElement<T>* PElement<T>::copie2(PElement<T>* l) {
	if (l == nullptr) return nullptr;
	else return new PElement<T>(copie2(l->suivant), new T(*(l->valeur)));
}
template<class T>
PElement<T>* PElement<T>::copie3(PElement<T>* l) {
	if (l == nullptr) return nullptr;
	else return new PElement<T>(copie3(l->suivant), l->valeur->clone());
}
template<class T>
PElement<T>* PElement<T>::appartient(const T* a, PElement<T>* l) {
	if (l == nullptr) return nullptr;
	else {
		if (l->valeur == a)
			return l;
		else
			return appartient(a, l->suivant);
	}
}

template<class T>
template<class FONCTEUR>
PElement<T>* PElement<T>::appartient(PElement<T>* l, const FONCTEUR& condition) {
	if (l == nullptr) return nullptr;
	if (condition(l->valeur)) return l;
	else return appartient(l->suivant, condition);//on suppose que la signature de condition est bool condition(const T *a)
}
