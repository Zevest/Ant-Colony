#include "vector.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//#define __VECTOR_DEBUG

Vector4_t *Vector4_zero()
{
    Vector4_t *v = (Vector4_t *)malloc(sizeof(Vector4_t));
    v->x = 0;
    v->y = 0;
    v->z = 0;
    v->w = 0;
    return v;
}
Vector4_t *Vector4_new(double x, double y, double z, double w)
{
    Vector4_t *v = (Vector4_t *)malloc(sizeof(Vector4_t));
    v->x = x;
    v->y = y;
    v->z = z;
    v->w = w;
    return v;
}
void Vector4_set(Vector4_t *v, double x, double y, double z, double w)
{
    v->x = x;
    v->y = y;
    v->z = z;
    v->w = w;
}

Vector3_t *Vector3_zero()
{
    Vector3_t *v = (Vector3_t *)malloc(sizeof(Vector3_t));
    v->x = 0;
    v->y = 0;
    v->z = 0;
    return v;
}

Vector3_t *Vector3_new(double x, double y, double z)
{
    Vector3_t *v = (Vector3_t *)malloc(sizeof(Vector3_t));

    v->x = x;
    v->y = y;
    v->z = z;
    return v;
}
void Vector3_set(Vector3_t *v, double x, double y, double z)
{
    v->x = x;
    v->y = y;
    v->z = z;
}

Vector2_t *Vector2_zero()
{
    Vector2_t *v = (Vector2_t *)malloc(sizeof(Vector2_t));
    v->x = 0;
    v->y = 0;
    return v;
}
Vector2_t *Vector2_new(double x, double y)
{
    Vector2_t *v = (Vector2_t *)malloc(sizeof(Vector2_t));
    v->x = x;
    v->y = y;
    return v;
}
void Vector2_set(Vector2_t *v, double x, double y)
{
    v->x = x;
    v->y = y;
}

void Vector_destroy(void *v)
{
    free(v);
    v = NULL;
}

double Vector2_dist(Vector2_t *a, Vector2_t *b)
{
    return sqrt((b->x - a->x) * (b->x - a->x) + (b->y - a->y) * (b->y - a->y));
}

double Vector3_dist(Vector3_t *a, Vector3_t *b)
{
    return sqrt((b->x - a->x) * (b->x - a->x) + (b->y - a->y) * (b->y - a->y) + (b->z - a->z) * (b->z - a->z));
}

double Vector4_dist(Vector4_t *a, Vector4_t *b)
{
    return sqrt((b->x - a->x) * (b->x - a->x) + (b->y - a->y) * (b->y - a->y) + (b->z - a->z) * (b->z - a->z) + (b->w - a->w) * (b->w - a->w));
}

#ifdef __VECTOR_DEBUG

int main()
{
    Vector2_t *v1 = Vector2_zero();
    Vector3_t *v2 = Vector3_zero();
    Vector4_t *v3 = Vector4_zero();
    printf("v1 (%f,%f), v2(%f, %f, %f) v3(%f, %f, %f, %f)\n",
           v1->x, v1->y, v2->x, v2->y, v2->z, v3->w, v3->x, v3->y, v3->z);
    Vector_destroy(v1);
    Vector_destroy(v2);
    Vector_destroy(v3);
    return 0;
}

#endif // !__VECTOR_DEBUG
