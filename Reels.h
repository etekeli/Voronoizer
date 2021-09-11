#ifndef REELS_H
#define REELS_H

#include <math.h>
#include <stdlib.h>

class Reels
{
public:
    constexpr static /*const*/ double PI = 3.1415926;

    /**
     *
     * RESULTAT : un nombre al�atoire x tel que 0 <= x < 1
     *
     * suppose qu'un appel � srand a �t� fait au pr�alable
     *
     * */
    inline static double random();

    /**
     *
     * RESULTAT : un nombre al�atoire x tel que | x - germe | < eps
     *
     * suppose qu'un appel � srand a �t� fait au pr�alable
     *
     * */
    inline static double random(const double& germe, const double& eps);

    /**
     *
     * RESULTAT : un nombre al�atoire x tel que | x - germe | < eps
     *
     * suppose qu'un appel � srand a �t� fait au pr�alable
     *
     * Pour �tre plac�e comme pointeur de fct param�tre d'une fct template
     *
     * */
    inline static const double random1(const double& germe, const double& eps) { return Reels::random(germe, eps); }
};


/**
 *
 * RESULTAT : un nombre al�atoire x tel que 0 <= x < 1
 *
 * suppose qu'un appel � srand a �t� fait au pr�alable
 *
 * */
inline /* static */ double Reels::random()
{
    return rand() / (double)RAND_MAX;
}


/**
 *
 * RESULTAT : un nombre al�atoire x tel que | x - germe | < eps
 *
 * suppose qu'un appel � srand a �t� fait au pr�alable
 * */
/* static */ inline double Reels::random(const double& germe, const double& eps)
{
    double t = Reels::random(); // 0 <= t <= 1

    return eps * (2 * t - 1) + germe;
}



#endif // REELS_H
