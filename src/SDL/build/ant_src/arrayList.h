#include <stdio.h>
#ifndef __ARRAYLIST_H__

typedef struct ArrayList
{
    size_t __item_size, count, __size;
    char *data;

} ArrayList_t;

typedef void (*Printer)(char *);

typedef int (*Comparator)(char *, char *);

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
int ArrayList_indexOf(ArrayList_t *array, char *val, Comparator comp);
int ArrayList_removeValue(ArrayList_t *array, char *val, Comparator comp);
int ArrayList_containValue(ArrayList_t *array, char *val, Comparator comp);

int ArrayList_shortComparator(char *a, char *b);
int ArrayList_intComparator(char *a, char *b);
int ArrayList_longComparator(char *a, char *b);
int ArrayList_floatComparator(char *a, char *b);
int ArrayList_doubleComparator(char *a, char *b);

void ArrayList_printShort(char *val);
void ArrayList_printInt(char *val);
void ArrayList_printLong(char *val);
void ArrayList_printFloat(char *val);
void ArrayList_printDouble(char *val);

#define __ARRAYLIST_H__
#endif // !_ARRAYLIST_H
