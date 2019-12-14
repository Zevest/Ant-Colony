#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "arrayList.h"
//#define __ARRAYLIST_DEBUG

ArrayList_t *ArrayList_new(size_t __size)
{
    ArrayList_t *array;
    array = malloc(sizeof *array);
    array->__item_size = __size;
    array->__size = 1;
    array->count = 0;
    array->data = malloc(array->__item_size);
    return array;
}

void ArrayList_destroy(ArrayList_t *array)
{
    free(array->data);
    array->data = NULL;
    free(array);
    array = NULL;
}
int __ArrayListGrow(ArrayList_t *array)
{
    char *data;
    assert(array);
    assert(array->data);
    data = malloc(array->__item_size * array->__size * 2);
    if (!data)
        return 1;
    memcpy(data, array->data, array->__size * array->__item_size);
    free(array->data);
    array->__size *= 2;
    array->data = data;
    return 0;
}

void ArrayList_add(ArrayList_t *array, char *val)
{
    assert(array);
    if (array->count == array->__size)
    {
        assert(__ArrayListGrow(array) == 0);
    }
    memcpy(array->data + (array->count++ * array->__item_size), val, array->__item_size);
}

void ArrayList_print(ArrayList_t *array, Printer print)
{
    assert(array);
    /*if(!array)
        return;*/
    int i;
    char *ptr;
    printf("[");
    for (i = 0, ptr = array->data; i < array->count; i++, ptr += array->__item_size)
    {
        print(ptr);
        if (i < array->count - 1)
            printf(",");
    }
    printf("]\n");
}

int __ArrayList_containIndex(ArrayList_t *array, int index)
{
    return (0 <= index && index <= array->count);
}

char *ArrayList_get(ArrayList_t *array, int index)
{
    assert(array);
    assert(array->data);
    //printf("%d within %ld : %s", index, array->count, __ArrayList_containIndex(array, index) ? "yes" : "no");
    assert(__ArrayList_containIndex(array, index) == 1 || array->count == 0);
    return (array->data + (index * array->__item_size));
}
void ArrayList_set(ArrayList_t *array, int index, char *val)
{
    assert(array);
    assert(array->data);
    assert(__ArrayList_containIndex(array, index) == 1 || array->count == 0);
    memcpy(ArrayList_get(array, index), val, array->__item_size);
}

void ArrayList_insert(ArrayList_t *array, int index, char *val)
{
    int i;
    assert(array);
    assert(array->data);
    assert(__ArrayList_containIndex(array, index) == 1 || array->count == 0);
    if (array->count == array->__size)
    {
        assert(__ArrayListGrow(array) == 0);
    }
    for (i = array->count; i >= index; i--)
    {
        if (i)
            ArrayList_set(array, i, ArrayList_get(array, i - 1));
    }
    ArrayList_set(array, index, val);
    array->count++;
}

int ArrayList_removeIndex(ArrayList_t *array, int index)
{
    assert(array);
    assert(array->data);
    assert(__ArrayList_containIndex(array, index));
    int i;
    for (i = index; i < array->count - 1; i++)
    {
        ArrayList_set(array, i, ArrayList_get(array, i + 1));
    }
    array->count--;
    return 0;
}

int ArrayList_indexOf(ArrayList_t *array, char *val, Comparator comp)
{
    assert(array);
    assert(array->data);
    int i;
    for (i = 0; i < array->count; i++)
    {
        if (comp(val, ArrayList_get(array, i)) == 0)
            return i;
    }
    return -1;
}

int ArrayList_removeValue(ArrayList_t *array, char *val, Comparator comp)
{
    assert(array);
    assert(array->data);
    int i = ArrayList_indexOf(array, val, comp);
    if (__ArrayList_containIndex(array, i))
    {
        ArrayList_removeIndex(array, i);
        return 0;
    }
    return 1;
}

int ArrayList_containValue(ArrayList_t *array, char *val, Comparator comp)
{
    assert(array);
    assert(array->data);
    return ArrayList_indexOf(array, val, comp) != -1;
}

int ArraryList_shortComparator(char *a, char *b)
{
    return (*(short *)a - *(short *)b);
}

int ArraryList_intComparator(char *a, char *b)
{
    return (*(int *)a - *(int *)b);
}

int ArraryList_longComparator(char *a, char *b)
{
    return (*(int *)a - *(int *)b);
}
int ArraryList_floatComparator(char *a, char *b)
{
    return (*(int *)a - *(int *)b);
}

int ArraryList_doubleComparator(char *a, char *b)
{
    return (*(int *)a - *(int *)b);
}

void ArrayList_printShort(char *val)
{
    printf("%d", *(short *)val);
}
void ArrayList_printInt(char *val)
{
    printf("%d", *(int *)val);
}
void ArrayList_printLong(char *val)
{
    printf("%ld", *(long *)val);
}
void ArrayList_printFloat(char *val)
{
    printf("%0.3f", *(float *)val);
}
void ArrayList_printDouble(char *val)
{
    printf("%0.6f", *(double *)val);
}

#ifdef __ARRAYLIST_DEBUG


void printDouble(char *val)
{
    printf("%0.3f", *(double *)val);
}

int compDouble(char *a, char *b)
{
    return (*(double *)a == *(double *)b);
}

int main()
{

    double val[50], i;
    ArrayList_t *array = ArrayList_new(sizeof i);
    for (i = 0; i < 25.0; i += 0.5)
    {
        val[(int)(i * 2)] = (5 * i) / 2;
        ArrayList_insert(array, ((int)(i * 2) % (array->count + 1) / 2), (char *)&val[(int)(i * 2)]);
    }
    i = 50;
    ArrayList_set(array, 0, (char *)&i);
    i = 6.250;
    printf("%0.3f is in the list ? %s.\n", i, ArrayList_containValue(array, (char *)&i, compDouble) ? "yes" : "no");
    ArrayList_Print(array, printDouble);
    ArrayList_removeValue(array, (char *)&i, compDouble);
    ArrayList_Print(array, printDouble);
    ArrayList_destroy(array);
}
#endif // !__ARRAYLIST_DEBUG
