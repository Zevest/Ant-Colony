#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "arrayList.h"
#include "random.h"
#include "assert.h"

int __Random_initialized = 0;

/**
 * Convertie d'un intervalle vers un autre 
 * @param val valeur a convertir
 * @param minSrc borne inférieur du premier intervalle
 * @param maxSrc borne supérieur du premier intervalle
 * @param minDest borne inférieur du second intervalle
 * @param maxDest borne supérieur du second intervalle
 * @return La valeur convertie dans le nouvel intervalle
 */
double map(double val, double minSrc, double maxSrc, double minDest, double maxDest)
{
    return val / (maxSrc - minSrc) * (maxDest - minDest) + minDest;
}

/** 
 * initialise la graine du générateur de nombre aléatoire
 */
void __Random_init()
{
    __Random_initialized = 1;
    srand(time(NULL));
}

/**
 * Génére un nombre aléatoire compris entre les bornes données
 * @param minV borne inférieur
 * @param maxV borne supérieur
 * @return le nombre aléatoire appartenant à l'intervalle 
 */
double Random_randRange(double minV, double maxV)
{
    if (!__Random_initialized)
        __Random_init();
    return map(rand(), 0, RAND_MAX, minV, maxV);
}

/**
 * Génerere  aléatoirement l'indice d'un choix Basé sur les probabilités données
 * @param weights la listes des probabilités des choix
 * @param len le nombre de choix
 * @return L'indice d'un choix en fonction la probabilité qu'il soit choisi
 */
int Random_weighted(double *weights, int len)
{
    if (!__Random_initialized)
        __Random_init();
    double max = 0, v;
    int i, j;
    ArrayList_t *coeff, *index;
    for (i = 0; i < len; ++i)
    {
        max += weights[i];
    }
    coeff = ArrayList_new(sizeof *weights);
    assert(coeff);
    for (i = 0; i < len; ++i)
    {
        v = map(weights[i], 0, max, 0.0, 1.0);
        ArrayList_add(coeff, (char *)&v);
    }

    index = ArrayList_new(sizeof(int));
    assert(index);
    for (i = 0; i < len; ++i)
    {
        v = (*(double *)ArrayList_get(coeff, i)) * 100;
        for (j = 0; j < (int)v; ++j)
        {
            ArrayList_add(index, (char *)&i);
        }
    }

    if (index->count > 0)
        i = *(int *)ArrayList_get(index, Random_randRange(0, index->count));
    else
        i = -1;
    //ArrayList_print(coeff, ArrayList_printDouble);
    //ArrayList_print(index, ArrayList_printInt);
    ArrayList_destroy(index);
    ArrayList_destroy(coeff);
    return i;
}
//#define __RANDOM_DEBUG
#ifdef __RANDOM_DEBUG
int main()
{
    double listA[10] = {0.00, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00, 9.00};
    double listB[10] = {20.0, 19.0, 18.0, 17.0, 16.0, 15.0, 14.0, 13.0, 12.0, 11.0};
    double proba[10] = {0.20, 0.15, 0.30, 0.42, 0.30, 0.20, 4.50, 0.02, 0.18, 0.24};
    int i;
    printf("map(18, 12, 20, 0, 100) = %0.2f\n", map(18, 12, 20, 0, 100));
    printf("randRange(17, 53) = %0.2f\n", Random_randRange(17, 53));
    printf("Random_weighted listA: %0.2f\n", listA[Random_weighted(proba, 10)]);
    printf("Random_weighted listB: %0.2f\n", listB[Random_weighted(proba, 10)]);
    for (i = 0; i < 100; i++)
    {
        printf("%0.2f ", listB[Random_weighted(proba, 10)]);
    }
    printf("\n");
    return 0;
}
#endif // !__RANDOM_DEBUG
