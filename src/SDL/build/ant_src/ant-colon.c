#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"
#include "arrayList.h"
#include "random.h"
#include "graph.h"

void print_branch(char *b)
{
	Vector4_t *branch = (Vector4_t *)b;
	printf("node(%.0f -> %.0f) len %.3f pheromone %.3f\n", branch->x, branch->y, branch->w, branch->z);
}

int BranchCompar(char *a, char *b)
{
	Vector4_t *branchA = (Vector4_t *)a;
	Vector4_t *branchB = (Vector4_t *)b;
	//if((int)a->x == (int)b->x && (int)a-<y == (int)b->y)
	return (int)branchA->x - (int)branchB->x + (int)branchA->y - (int)branchB->y;
}

ArrayList_t *chemin_connecte(Graph g, int nodeNum, ArrayList_t *tabou)
{
	int i, tmpA, tmpB;
	ArrayList_t *connectedPath = ArrayList_new(sizeof(Vector4_t));

	for (i = 0; i < g->liste_arc->count; ++i)
	{
		Vector4_t *branch = (Vector4_t *)ArrayList_get(g->liste_arc, i);
		//index = ArrayList_indexOf(g->liste_arc, (char*)branch, BranchCompar);
		tmpA = (int)branch->x;
		tmpB = (int)branch->y;
		if (tmpA == nodeNum && !ArrayList_containValue(tabou, (char *)&tmpB, ArrayList_intComparator))
		{
			ArrayList_add(connectedPath, (char *)branch);
		}
		else if (tmpB == nodeNum && !ArrayList_containValue(tabou, (char *)&tmpA, ArrayList_intComparator))
		{
			ArrayList_add(connectedPath, (char *)branch);
		}
	}
	//ArrayList_print(connectedPath, print_branch);
	return connectedPath;
}

void deplace_fourmis(Graph g, int nodeNum, int *count)
{
	int j, i, choosed_number, antNum = 2 * nodeNum, oldNode, index;
	ArrayList_t *validPath;
	Vector4_t *tmp;
	double *weights;
	for (j = 0; j < 2; ++j)
	{
		validPath = chemin_connecte(g, g->fourmis[antNum + j].node, (g->fourmis + antNum + j)->tabou);
		//ArrayList_print(validPath, print_branch);
		//ArrayList_print((g->fourmis + antNum + j)->tabou, ArrayList_printInt);
		if (validPath->count > 0)
		{ // si il existe un chemin
			weights = malloc(validPath->count * sizeof *weights);
			for (i = 0; i < validPath->count; ++i)
			{
				tmp = (Vector4_t *)ArrayList_get(validPath, i);
				//printf("Path from %d to %d\n", (int) tmp->x, (int) tmp->y);
				double nWeight = tmp->z / pow(tmp->w, 7) * 1000;
				//printf("weights = %.20f\n", nWeight);
				weights[i] = nWeight;
			}
			choosed_number = Random_weighted(weights, validPath->count);
			// printf("Choosed Number = %d\n", choosed_number);
			tmp = (Vector4_t *)ArrayList_get(validPath, choosed_number);
			index = ArrayList_indexOf(g->liste_arc, (char *)tmp, BranchCompar);
			if (index >= 0 && index < g->liste_arc->count)
				++count[index];
			oldNode = g->fourmis[antNum + j].node;
			ArrayList_add((g->fourmis + antNum + j)->tabou, (char *)&oldNode);
			if (oldNode == (int)tmp->x)
				g->fourmis[antNum + j].node = (int)tmp->y;
			else
				g->fourmis[antNum + j].node = (int)tmp->x;
			//printf("moved Ant %d from %d to %d\n", antNum + j, oldNode, g->fourmis[antNum + j].node);
			free(weights);
		}
		ArrayList_destroy(validPath);
	}
	//printf("END\n");
}

int get_shortest_path_index(Graph g)
{
	int bestIndex, i, j, *tabou;
	double minSum = INFINITY, sum;
	Vector4_t *tmp;
	for (j = 0; j < g->nb_vertices * 2; ++j)
	{
		sum = 0;

		for (i = 0, tabou = (int *)ArrayList_get((g->fourmis + j)->tabou, i); i < g->fourmis[j].tabou->count; ++tabou, ++i)
		{

			tmp = (Vector4_t *)ArrayList_get(g->liste_arc, *tabou);
			sum += tmp->w;
		}
		//printf("Path %d length %f\n", j, sum);
		if (sum < minSum)
		{
			minSum = sum;
			bestIndex = j;
		}
	}
	printf("Best index: %d\n", bestIndex);
	return bestIndex;
}

int main()
{
	Vector4_t *arrete;
	Node start, end;
	ArrayList_t *shortest;
	int total = 5, *nb_passage;
	Graph g = new_graph(total);
	int i, j = 2;
	double pheromone;

	for (i = 0; i < total; ++i)
	{
		for (j = i + 1; j < total; ++j)
		{
			add_edge(g, i, j);
		}
	}

	for (i = 0; i < g->liste_arc->count; i++)
	{
		arrete = (Vector4_t *)ArrayList_get(g->liste_arc, i);
		start = (Node)ArrayList_get(g->nodelist, (int)arrete->x);
		end = (Node)ArrayList_get(g->nodelist, (int)arrete->y);
		arrete->w = Vector2_dist(&start->pos, &end->pos);
	}
	//print_graph(g);
	//moveAnt(g, 0);
	//ArrayList_t * chemin = chemin_connecte(g , 0, g->fourmis[0].tabou);
	nb_passage = malloc(g->liste_arc->count * sizeof *nb_passage);
	for (i = 0; i < g->liste_arc->count; ++i)
	{
		nb_passage[i] = 0;
	}
	for (i = 0; i < total; ++i)
	{
		for (j = 0; j < total; ++j)
		{
			deplace_fourmis(g, j, nb_passage);
		}
		//printf("\n[");
		for (j = 0; j < g->liste_arc->count; ++j)
		{
			arrete = (Vector4_t *)ArrayList_get(g->liste_arc, j);
			pheromone = (arrete->z / 2.0) + 100.0 * nb_passage[j];
			//printf("%d:  %f  :%f\n,", nb_passage[j], arrete->z, pheromone);
			nb_passage[j] = 0;
			arrete->z = pheromone;
		}
		//printf("]\n");
	}

	shortest = g->fourmis[get_shortest_path_index(g)].tabou;
	//printf("the shortest Path is :");
	ArrayList_print(shortest, ArrayList_printInt);

	free(nb_passage);
	erase_graph(g);
}
