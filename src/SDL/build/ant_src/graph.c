#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "arrayList.h"
#include "random.h"
#include "graph.h"
//#define __GRAPH_DEBUG
/**
* Créée un Graphe
* @param vertices Le nombre de sommets
* @param is_oriented Est-ce un graphe orienté ou non-orienté ?
* @return Le graphe créé
*/
Graph new_graph(int vertices, Bool is_oriented)
{

	int i;

	Graph element;
	element = malloc(sizeof(*element));
	if (element == NULL)
	{
		fprintf(stderr, "Erreur : Probleme creation Graphe.\n");
		exit(EXIT_FAILURE);
	}
	element->fourmis = malloc(2 * vertices * sizeof *element->fourmis);
	element->is_oriented = is_oriented;
	element->nb_vertices = vertices;
	element->liste_arc = ArrayList_new(sizeof(Vector4_t));
	element->nodelist = ArrayList_new(sizeof(NodeListElement));

	//element->tab_neighbours = malloc(vertices * sizeof(AdjencyListElement));
	//Node empty = malloc(sizeof(NodeListElement));
	for (i = 0; i < vertices; i++)
	{
		Node n = add_node(i) ;
		ArrayList_add(element->nodelist,(char *)n);
		element->fourmis[2 * i].node = i;
		element->fourmis[2 * i + 1].node = i;
		element->fourmis[2 * i].tabou = ArrayList_new(sizeof i);
		element->fourmis[2 * i + 1].tabou = ArrayList_new(sizeof i);
		ArrayList_add(element->fourmis[2*i].tabou,(char*) &i);
		ArrayList_add(element->fourmis[2*i+1].tabou,(char*) &i);
		//ArrayList_add(element->nodelist, (char *)empty);
	}
	//free(empty);
	/* 
	}
	if(element->liste_arc == NULL)
	{
		fprintf(stderr, "Erreur : Probleme creation Graphe.\n");
		exit(EXIT_FAILURE);
	}
	for(i = 1 ; i < element->nb_vertices + 1 ; i++)
		element->tab_neighbours[i-1].begin = NULL;

	if(element->is_oriented)
		element->graph_file = fopen("digraph.out", "w");
	else
		element->graph_file = fopen("graph.out", "w");

	if(element->graph_file == NULL)
	{
		fprintf(stderr, "Erreur : Probleme creation du fichier.\n");
		exit(EXIT_FAILURE);
	}

	if(element->is_oriented)
		fprintf(element->graph_file, "digraph my_graph\n{\n");
	else
		fprintf(element->graph_file, "graph my_graph\n{\n");
*/
	return element;
}

/*----------------------------------------------------------------------------------------------*/

/**
* Teste si un Graphe existe
* @param g Le Graphe à vérifier
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
* @param x La valeur du Noeud
* @param Le Noeud créé
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

/*----------------------------------------------------------------------------------------------*/

/**
* Ajoute une arête à un Graphe
* @param g Le Graphe
* @param src Le premier sommet (ou source)
* @param dest Le second sommet (ou destination)
*/
void add_edge(Graph g, int src, int dest)
{
	//Node n = add_node(src);
	Vector4_t branch;
	Vector4_set(&branch, src, dest, Random_randRange(0, 1), 0);
	ArrayList_add(g->liste_arc, (char *)&branch);
	//ArrayList_set(g->nodelist, src - 1, (char *)n);
	//free(n);

}

/*----------------------------------------------------------------------------------------------*/

/**
* Affiche un Graphe en mode console
* @param g Le Graphe
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
* Affiche un Graphe en 2D (avec Graphviz sous WINDOWS)
* @param g Le Graphe
*/
void display_graph(Graph g)
{
	//Windows seulement !
	if (g->is_oriented)
		system("%CD%/graphviz/bin/dotty.exe digraph.out");
	else
		system("%CD%/graphviz/bin/dotty.exe graph.out");
}



/*----------------------------------------------------------------------------------------------*/

/**
* Supprime un Graphe
* @param g Le Graphe
*
void erase_graph(Graph g)
{
	if(is_empty_graph(g))
	{
		printf("Rien a effacer, le Graphe n'existe pas.\n");
		return;
	}

	//Si il existe des sommets adjacents
	if(g->tab_neighbours)
	{
		int i;

		for(i = 1 ; i < g->nb_vertices + 1 ; i++)
		{
			NodeList n = g->tab_neighbours[i-1].begin;
			
			while(n != NULL)
			{
				NodeList tmp = n;
				n = n->next;
				free(tmp);
			}
		}

		//Libération de la liste d'adjacence
		free(g->tab_neighbours);
	}

	//Fin et fermeture du fichier Graphviz
	fprintf(g->graph_file, "}\n");
	fclose(g->graph_file);

	//Libération du Graphe de la mémoire
	free(g);
}*/


#ifdef __GRAPH_DEBUG
int main(int argc, char const *argv[])
{
	Vector4_t *arrete;
	Node start, end;


	Graph g = new_graph(6, false);
	int i, j = 2;
	//for (i = 1; i < 30; i++)
	//{
		//printf("Doing it for %d times.\n", i);
		//add_edge(g, 1, 2);
		//add_edge(g, 1, 3);
		////add_edge(g, 3, 4);
		//add_edge(g, 4, 5);
		//add_edge(g, 1, 0);
		//printf("Then\n");
	//	j++;
	//}
	
	for (i = 1; i < 6; i++)
	{
		arrete = (Vector4_t*)ArrayList_get(g->liste_arc, i);
		start = (Node)ArrayList_get(g->nodelist,(int) arrete->x);
		end = (Node)ArrayList_get(g->nodelist,(int) arrete->y);
		arrete->w = Vector2_dist(&start->pos, &end->pos);
		//Vector4_set(arrete, arrete->x, arrete->y, arrete->z, dist);		
		
	}
	print_graph(g);
	/*
	for (i = 0; i < g->nodelist->count; i++)
	{
		Node n = (Node)ArrayList_get(g->nodelist, i);
		if (n)
		{
			free(n);
		}
	}
	for (i = 0; i < g->liste_arc->count; i++)
	{
		free((Vector4_t *)ArrayList_get(g->nodelist, i));
	}*/

	ArrayList_destroy(g->nodelist);
	ArrayList_destroy(g->liste_arc);
	free(g->fourmis);
	free(g);
	return 0;
}
#endif // !__GRAPH_DEBUG