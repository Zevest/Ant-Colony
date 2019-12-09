#include <stdio.h>
#ifndef _ARRAYLIST_H
#define _ARRAYLIST_H
typedef struct ArrayList
{
    size_t __item_size, count, __size;
    char *data;

} ArrayList_t;

typedef void (*Printer)(char *);

typedef int (*Equals)(char *, char *);

ArrayList_t *ArrayList_new(size_t __size);
void ArrayList_destroy(ArrayList_t *array);
int __ArrayListGrow(ArrayList_t *array);
int __ArrayList_containIndex(ArrayList_t *array, int index);

void ArrayList_add(ArrayList_t *array, char *val);
void ArrayList_insert(ArrayList_t *array, int index, char *val);

char *ArrayList_get(ArrayList_t *array, int index);
void ArrayList_set(ArrayList_t *array, int index, char *val);

void ArrayList_print(ArrayList_t *array, Printer print);

int ArrayList_removeIndex(ArrayList_t *array, int index);
int ArrayList_indexOf(ArrayList_t *array, char *val, Equals equal);
int ArrayList_removeValue(ArrayList_t *array, char *val, Equals equal);
int ArrayList_containValue(ArrayList_t *array, char *val, Equals equal);

#endif // !_ARRAYLIST_H
