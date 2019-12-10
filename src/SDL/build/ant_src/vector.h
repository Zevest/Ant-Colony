#ifndef __VECTOR_H__
typedef struct Vector4_t
{
    double x, y, z, w;
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
Vector4_t *Vector4_new(double x, double y, double z, double w);
void Vector4_set(Vector4_t *v, double x, double y, double z, double w);
double Vector4_dist(Vector4_t *a, Vector4_t *b);

Vector3_t *Vector3_zero();
Vector3_t *Vector3_new(double x, double y, double z);
void Vector3_set(Vector3_t *v, double x, double y, double z);
double Vector3_dist(Vector3_t *a, Vector3_t *b);

Vector2_t *Vector2_zero();
Vector2_t *Vector2_new(double x, double y);
void Vector2_set(Vector2_t *v, double x, double y);
double Vector2_dist(Vector2_t *a, Vector2_t *b);

void Vector_destroy(void *v);
#define __VECTOR_H__
#endif // !__VECTOR_H__
