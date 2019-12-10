#ifndef __RANDOM_H__

double map(double val, double minSrc, double maxSrc, double minDest, double maxDest);

void __Random_init();

double Random_randRange(double minV, double maxV);

int Random_weighted(double weights[], int len);

#define __RANDOM_H__
#endif // !__RANDOM_H__
