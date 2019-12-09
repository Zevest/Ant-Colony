#ifndef _RANDOM_H
#define _RANDOM_H

double map(double val, double minSrc, double maxSrc, double minDest, double maxDest);

void __Random_init();

double Random_randRange(double minV, double maxV);

int Random_weighted(double weights[], int len);

#endif // !_RANDOM_H