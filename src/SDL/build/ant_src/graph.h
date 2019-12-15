#ifndef __GRAPH_H__
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

struct fourmis_t
{
	int node;
	ArrayList_t *tabou;
};
typedef struct fourmis_t fourmis_t;
/* Définition d'une liste d'ajdacence (tableau) 
	typedef struct AdjencyListElement
	{
		NodeListElement *begin;
	}AdjencyListElement, *AdjencyList;

	 Définition d'un Graphe */
typedef struct GraphElement
{
	int nb_vertices, antPerNode;
	fourmis_t *fourmis;
	ArrayList_t *liste_arc;
	ArrayList_t *nodelist;
	//AdjencyList tab_neighbours;
	//FILE *graph_file;
} GraphElement, *Graph;

/* Prototypes */
Graph new_graph(int vertices, int antPerNode);
Bool is_empty_graph(Graph g);
Node add_node(int x);
void add_edge(Graph g, int src, int dest);
void print_graph(Graph g);
void erase_graph(Graph g);

#define __GRAPH_H__
#endif //! __GRAPH_H__
