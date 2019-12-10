#ifndef __GRAPH__H__
#include "vector.h"
#include "arrayList.h"
/* Définition d'un Booléen */
typedef enum
{
	false,
	true
} Bool;

/* Définition d'une liste de Noeuds (sommet) */
typedef struct NodeListElement
{
	int value;
	Vector2_t pos;
	//struct NodeListElement *listenode;
} NodeListElement, *Node;

/* Définition d'une liste d'ajdacence (tableau) 
	typedef struct AdjencyListElement
	{
		NodeListElement *begin;
	}AdjencyListElement, *AdjencyList;

	 Définition d'un Graphe */
typedef struct GraphElement
{
	Bool is_oriented;
	int nb_vertices;
	int *fourmis;
	ArrayList_t *liste_arc;
	ArrayList_t *nodelist;
	//AdjencyList tab_neighbours;
	FILE *graph_file;
} GraphElement, *Graph;

/* Prototypes */
Graph new_graph(int vertices, Bool is_oriented);
Bool is_empty_graph(Graph g);
Node add_node(int x);
void add_edge(Graph g, int src, int dest);
void print_graph(Graph g);
void display_graph(Graph g);
void erase_graph(Graph g);

#define __GRAPH__H__
#endif
