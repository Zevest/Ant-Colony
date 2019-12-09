#ifndef _VECTOR_H
#define _VECTOR_H
typedef struct Vector4_t
{
    double w, x, y, z;
} Vector4_t;
typedef struct Vector3_t
{
    double x, y, z;
} Vector3_t;
typedef struct Vector2_t
{
    double x, y;
} Vector2_t;

Vector4_t *Vector4_zero();
Vector4_t *Vector4_new(double w, double x, double y, double z);

Vector3_t *Vector3_zero();
Vector3_t *Vector3_new(double x, double y, double z);

Vector2_t *Vector2_zero();
Vector2_t *Vector2_new(double x, double y);

void Vector_destroy(void *v);
#endif // !_VECTOR_H
