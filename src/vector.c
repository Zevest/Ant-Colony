#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

//#define __VECTOR_DEBUG

Vector4_t *Vector4_zero()
{
    Vector4_t *v = (Vector4_t *)malloc(sizeof(Vector4_t));
    v->w = 0;
    v->x = 0;
    v->y = 0;
    v->z = 0;
    return v;
}
Vector4_t *Vector4_new(double w, double x, double y, double z)
{
    Vector4_t *v = (Vector4_t *)malloc(sizeof(Vector4_t));
    v->w = w;
    v->x = x;
    v->y = y;
    v->z = z;
    return v;
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

void Vector_destroy(void *v)
{
    free(v);
    v = NULL;
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
