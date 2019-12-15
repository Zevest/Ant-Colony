#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "arrayList.h"
#include "random.h"
#include "graph.h"
//#define __GRAPH_DEBUG
/**
* Créée un Graphe
* vertices Le nombre de sommets
* is_oriented Est-ce un graphe orienté ou non-orienté ?
* return Le graphe créé
*/
Graph new_graph(int vertices, int antPerNode)
{

	int i, j;

	Graph element;
	element = malloc(sizeof(*element));
	if (element == NULL)
	{
		fprintf(stderr, "Erreur : Probleme creation Graphe.\n");
		exit(EXIT_FAILURE);
	}
	element->antPerNode = antPerNode;
	element->fourmis = malloc(element->antPerNode * vertices * sizeof *element->fourmis);
	element->nb_vertices = vertices;
	element->liste_arc = ArrayList_new(sizeof(Vector4_t));
	element->nodelist = ArrayList_new(sizeof(NodeListElement));

	for (i = 0; i < vertices; i++)
	{
		Node n = add_node(i);
		ArrayList_add(element->nodelist, (char *)n);
		free(n);
		for (j = 0; j < element->antPerNode; ++j)
		{
			element->fourmis[element->antPerNode * i + j].node = i;
			element->fourmis[element->antPerNode * i + j].tabou = ArrayList_new(sizeof i);
		}
	}
	//ArrayList_add(element->fourmis[2*i].tabou,(char*) &i);
	//ArrayList_add(element->fourmis[2*i+1].tabou,(char*) &i);
	return element;
}

/*----------------------------------------------------------------------------------------------*/

/**
* Teste si un Graphe existe
* g Le Graphe à vérifier
* @return true Si le Graphe n'existe pas, false sinon
*/
Bool is_empty_graph(Graph g)
{
	if (g == NULL)
		return true;

	return false;
}

/*----------------------------------------------------------------------------------------------*/

/**
* Retourne un Noeud
* x La valeur du Noeud
* Le Noeud créé
*/
Node add_node(int x)
{
	Node n = malloc(sizeof(NodeListElement));

	if (n == NULL)
	{
		fprintf(stderr, "Erreur : Probleme creation Node.\n");
		exit(EXIT_FAILURE);
	}

	n->value = x;
	Vector2_set(&n->pos, (int)Random_randRange(1, 30), (int)Random_randRange(1, 30));

	return n;
}

void graph_free(Graph g)
{
}

/*----------------------------------------------------------------------------------------------*/

/**
* Ajoute une arête à un Graphe
* g Le Graphe
* src Le premier sommet (ou source)
* dest Le second sommet (ou destination)
*/
void add_edge(Graph g, int src, int dest)
{
	Vector4_t branch;
	Vector4_set(&branch, src, dest, Random_randRange(0, 1), 0);
	ArrayList_add(g->liste_arc, (char *)&branch);
}

/*----------------------------------------------------------------------------------------------*/

/**
* Affiche un Graphe 
*/
void print_graph(Graph g)
{
	int i;
	for (i = 1; i < g->nb_vertices + 1; i++)
	{
		Node n = (Node)ArrayList_get(g->nodelist, i - 1);
		printf("(%d): ", i);
		if (n != NULL)
		{
			printf("x:%.1f, y:%.1f, ", n->pos.x, n->pos.y);
			printf("val: %d\n", n->value);
		}
		else
			printf("pos(NULL), val; NULL\n");
	}
	for (i = 0; i < g->liste_arc->count; ++i)
	{
		Vector4_t branch = *(Vector4_t *)ArrayList_get(g->liste_arc, i);
		printf("Arrête(%d, %d)\n	distance: %0.3f\n	phéromone: %0.3f\n", (int)branch.x, (int)branch.y, branch.w, branch.z);
	}
}

/*----------------------------------------------------------------------------------------------*/

/**
* Supprime un Graphe
* @param g Le Graphe
*/
void erase_graph(Graph g)
{
	ArrayList_destroy(g->nodelist);
	ArrayList_destroy(g->liste_arc);
	int i;
	for (i = 0; i < g->nb_vertices * g->antPerNode; ++i)
		ArrayList_destroy((g->fourmis + i)->tabou);
	free(g->fourmis);
	free(g);
}

#ifdef __GRAPH_DEBUG
int main(int argc, char const *argv[])
{
	Vector4_t *arrete;
	Node start, end;
	Graph g = new_graph(6, false);
	int i, j = 2;
	for (i = 0; i < 6; i++)
	{

		arrete = (Vector4_t *)ArrayList_get(g->liste_arc, i);
		start = (Node)ArrayList_get(g->nodelist, (int)arrete->x);
		end = (Node)ArrayList_get(g->nodelist, (int)arrete->y);
		arrete->w = Vector2_dist(&start->pos, &end->pos);
		printf("Setting val for arrete[%d] to %f", i, Vector2_dist(&start->pos, &end->pos));
		//Vector4_set(arrete, arrete->x, arrete->y, arrete->z, dist);
	}
	print_graph(g);

	ArrayList_destroy(g->nodelist);
	ArrayList_destroy(g->liste_arc);
	free(g->fourmis);
	free(g);
	return 0;
}
#endif // !__GRAPH_DEBUG
