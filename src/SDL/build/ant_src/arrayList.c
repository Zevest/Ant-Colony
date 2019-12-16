#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "arrayList.h"
//#define __ARRAYLIST_DEBUG

/**
 * Creation d'une Liste à taille Dynamic
 * @param __size La taille du type de variable que l'on stock dans la liste
 * @return un pointeur vers la liste
 */
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
/**
 * Copie une Arrayliste_t
 * @param array addresse de l'ArrayList
 * @return un pointeur vers l'ArrayList crée
 */
ArrayList_t *ArrayList_copy(ArrayList_t *array)
{
    int i;
    ArrayList_t *cpy = ArrayList_new(array->__item_size);
    cpy->count = 0;
    for (i = 0; i < array->count; ++i)
    {
        ArrayList_add(cpy, ArrayList_get(array, i));
    }
    return cpy;
}

/**
 * Libere la memoire allouée a l'ArrayList
 *  @param array addresse de l'ArrayList
 */
void ArrayList_destroy(ArrayList_t *array)
{
    free(array->data);
    array->data = NULL;
    free(array);
    array = NULL;
}

/**
 * Double la memoire allouée pour stocker les données
 * @param array addresse de l'ArrayList
 * @return 0 si tout sc'est bien passé
 */
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

/**
 * Ajoute une valeur a l'ArrayList
 * @param array addresse de l'ArrayList
 * @param val address de la valeur caster en (char *)
 */
void ArrayList_add(ArrayList_t *array, char *val)
{
    assert(array);
    if (array->count == array->__size)
    {
        assert(__ArrayListGrow(array) == 0);
    }
    memcpy(array->data + (array->count++ * array->__item_size),
           val, array->__item_size);
}

/**
 * Afficher tous les élément de l'ArrayListe 
 * @param param addresse de l'ArrayList
 * @param print address d'une fonction qui affiche un élément de l'ArrayList ()
 */
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

/**
 * Verifie que l'indice est bien contenue dans l'ArrayList
 * @param array addresse de l'ArrayList
 * @param index indice
 * @return 1 si l'indice est compris entre 0 et le nombre d'éléments contenu dans la liste
 */
int __ArrayList_containIndex(ArrayList_t *array, size_t index)
{
    return (0 <= index && index <= array->count);
}

/**
 * Calcule l'address d'un élément stocké dans l'ArrayList
 * @param array addresse de l'ArrayList
 * @param index Indice de l'élément
 * @return l'address de la élément
 */
char *ArrayList_get(ArrayList_t *array, size_t index)
{
    assert(array);
    assert(array->data);
    if (!__ArrayList_containIndex(array, index) == 1 && !array->count == 0)
    {
        fprintf(stderr,
                "Error: index out of Range, try to get index %ld in a list of range %ld\n",
                index, array->count);
    }
    assert(__ArrayList_containIndex(array, index) == 1 || array->count == 0);
    return (array->data + (index * array->__item_size));
}

/**
 * Change dans l'ArrayList la valeur stockée à l'indice donnée par la valeur passé en argument
 * @param array addresse de l'ArrayList
 * @param index Indice de l'élément
 * @param val address de la valeur caster en (char *)
 */
void ArrayList_set(ArrayList_t *array, size_t index, char *val)
{
    assert(array);
    assert(array->data);
    assert(__ArrayList_containIndex(array, index) == 1 || array->count == 0);
    memcpy(ArrayList_get(array, index), val, array->__item_size);
}

/**
 * Insert dans l'ArrayList la valeur passée en argument à l'indice donnée
 * @param array addresse de l'ArrayList
 * @param index Indice de l'insertion
 * @param val address de la valeur caster en (char *)
 */
void ArrayList_insert(ArrayList_t *array, size_t index, char *val)
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

/**
 * Enlève un élément de l'ArrayList à l'indice donnée
 * @param array addresse de l'ArrayList
 * @param index Indice de l'insertion
 * @return renvoie 0 si l'élément est retirer sans encombre
 */
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

/**
 * Récuperer l'indice du premier élément équivalent  à la valeur donnée selon le comparateur
 * @param array addresse de l'ArrayList
 * @param val la valeur à retrouver
 * @param comp Comparateur d'élément
 * @return L'indice de l'élément si il existe sinon -1
 */
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

/**
 * Supprime le premier élément équivalent à la valeur donnée selon le comparateur
 * @param array addresse de l'ArrayList
 * @param val la valeur à retirer
 * @param comp Comparateur d'élément
 * @return L'indice de l'élément si il existe sinon -1
 */
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

/**
 * Vérifie si un élément de l'ArrayList équivaut à celui donné selon le comparateur
 * @param array addresse de l'ArrayList
 * @param val la valeur à retirer
 * @param comp Comparateur d'élément 
 */
int ArrayList_containValue(ArrayList_t *array, char *val, Comparator comp)
{
    assert(array);
    assert(array->data);
    return ArrayList_indexOf(array, val, comp) != -1;
}

/**
 * Comparateur de short
 * @param a address d'un short caster en (char *)
 * @param b address d'un short caster en (char *)
 * @return a - b
 */
int ArrayList_shortComparator(char *a, char *b)
{
    return (*(short *)a - *(short *)b);
}

/**
 * Comparateur de int
 * @param a address d'un int caster en (char *)
 * @param b address d'un int caster en (char *)
 * @return a - b
 */
int ArrayList_intComparator(char *a, char *b)
{
    return (*(int *)a - *(int *)b);
}

/**
 * Comparateur de long
 * @param a address d'un long caster en (char *)
 * @param b address d'un long caster en (char *)
 * @return a - b
 */
int ArrayList_longComparator(char *a, char *b)
{
    return (*(int *)a - *(int *)b);
}

/**
 * Comparateur de float
 * @param a address d'un float caster en (char *)
 * @param b address d'un float caster en (char *)
 * @return a - b
 */
int ArrayList_floatComparator(char *a, char *b)
{
    return (*(int *)a - *(int *)b);
}

/**
 * Comparateur de double
 * @param a address d'un double caster en (char *)
 * @param b address d'un double caster en (char *)
 * @return a - b
 */
int ArrayList_doubleComparator(char *a, char *b)
{
    return (*(int *)a - *(int *)b);
}

/**
 * Affiche un short avec printf
 * @param val address d'un short caster en (char *)
 */
void ArrayList_printShort(char *val)
{
    printf("%d", *(short *)val);
}

/**
 * Affiche un int avec printf
 * @param val address d'un int caster en (char *)
 */
void ArrayList_printInt(char *val)
{
    printf("%d", *(int *)val);
}

/**
 * Affiche un long avec printf
 * @param val address d'un long caster en (char *)
 */
void ArrayList_printLong(char *val)
{
    printf("%ld", *(long *)val);
}

/**
 * Affiche un float avec printf
 * @param val address d'un float caster en (char *)
 */
void ArrayList_printFloat(char *val)
{
    printf("%0.3f", *(float *)val);
}

/**
 * Affiche un double avec printf
 * @param val address d'un double caster en (char *)
 */
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
