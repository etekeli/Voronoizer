/*

Test des méthodes de base sur un graphe sauf les opérations de dessin

L'info associée aux sommets est un string  (par exemple)
L'info associée aux arêtes est un char     (par exemple)

*//*
#include <iostream>
#include <string>
#include "Graphe.h"

using namespace std;

int main()
{
	char ch;


	Graphe<char, string> g2;			// pour vérifier que l'opérateur = et que le destructeur fonctionnent bien

	{
		Graphe<char, string> g1;	// création à vide
		Sommet<string>* s0, * s1, * s2, * s3;

		//------------------ on insère des nouveaux sommets isolés --------------------

		s0 = g1.creeSommet("King's Landing");
		s1 = g1.creeSommet("Winterfell");
		s2 = g1.creeSommet("DragonStone");
		s3 = g1.creeSommet("The wall");

		//----------------- on connecte certains sommets -------------------

		Arete<char, string>* a0, * a1, * a2, * a3;

		a0 = g1.creeArete('a', s1, s0);
		a1 = g1.creeArete('b', s2, s1);
		a2 = g1.creeArete('c', s3, s2);
		a3 = g1.creeArete('d', s3, s1);

		//------------------ faire le dessin du graphe sur papier en notant les noms et les degrés pour comprendre la suite ----------------


		cout << "le graphe créé g1 est :" << endl << g1 << endl; cin >> ch;

		cout << "le nombre de sommets de g1 est : " << g1.nombreSommets() << endl;
		cout << "le nombre d'arêtes de g1 est : " << g1.nombreAretes() << endl; cin >> ch;

		PElement<Sommet<string>>* l0 = g1.voisins(s0);
		cout << "la liste des voisins de s0 est : " << endl << l0 << endl; cin >> ch;

		PElement<Arete<char, string>>* adj0 = g1.aretesAdjacentes(s0);
		cout << "la liste des arêtes adjacentes à s0 est : " << endl << adj0 << endl; cin >> ch;

		PElement<Sommet<string>>* l1 = g1.voisins(s1);
		cout << "la liste des voisins de s1 est : " << endl << l1 << endl;

		PElement<Arete<char, string>>* adj1 = g1.aretesAdjacentes(s1);
		cout << "la liste des arêtes adjacentes à s1 est : " << endl << adj1 << endl; cin >> ch;

		Arete<char, string>* a = g1.getAreteParSommets(s1, s3);

		cout << "l'arête joignant s1 et s3 est : " << endl << *a << endl; cin >> ch;



		g2 = g1;
	}			// à la fin de ce bloc, le destructeur est appelé pour g1.
				// cela permet de vérifier que l'op = a fait une vraie copie de g1

	cout << "le graphe créé g2 comme copie de g1 est :" << endl << g2 << endl; cin >> ch;


	return 0;
}
*/