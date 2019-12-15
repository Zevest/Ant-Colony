#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"
#include "arrayList.h"
#include "random.h"
#include "graph.h"
#define __ANT_COLON_DEBUG
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
	int j, i, choosed_number = 0, antNum = g->antPerNode * nodeNum, oldNode, index = 0;
	ArrayList_t *validPath;
	Vector4_t *tmp = NULL;
	double *weights;
	weights = malloc(g->nodelist->count * sizeof *weights);
	for (i = 0; i < g->nodelist->count; weights[i++] = 0)
		;
	for (j = 0; j < g->antPerNode; ++j)
	{
		validPath = chemin_connecte(g, g->fourmis[antNum + j].node, (g->fourmis + antNum + j)->tabou);
		//ArrayList_print(validPath, print_branch);
		//ArrayList_print((g->fourmis + antNum + j)->tabou, ArrayList_printInt);
		if (validPath->count > 0)
		{ // si il existe un chemin
			//weights =
			for (i = 0; i < validPath->count; ++i)
			{

				//if (i < 0 || i > validPath->count)
				//printf("%d,", i);
				tmp = (Vector4_t *)ArrayList_get(validPath, i);
				double nWeight = tmp->z / pow(tmp->w, 7) * 1000;
				weights[i] = nWeight;
			}
			choosed_number = Random_weighted(weights, validPath->count);
			if (choosed_number >= 0)
			{
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
			}
			//printf("moved Ant %d from %d to %d\n", antNum + j, oldNode, g->fourmis[antNum + j].node);
		}
		ArrayList_destroy(validPath);
	}
	free(weights);
	//printf("END\n");
}

double calDist(ArrayList_t *branch, ArrayList_t *path)
{
	int *pathPoints, i;
	Vector4_t *tmp = NULL;
	double sum = 0;
	//printf("Got Path: %ld %ld\n", branch->count, path->count);
	for (i = 0, pathPoints = (int *)ArrayList_get(path, 0); i < path->count; ++pathPoints, ++i)
	{
		tmp = (Vector4_t *)ArrayList_get(branch, *pathPoints);
		sum += tmp->w;
		//printf("tmp->w %f\n", tmp->w);
	}
	//printf("ouf\n");
	return sum;
}

int get_shortest_path_index(Graph g)
{
	int bestIndex = 0, i, j, *tabou;
	Bool done = false;
	double minSum = INFINITY, sum;
	Vector4_t *tmp;
	for (j = 0; j < g->nb_vertices * g->antPerNode; ++j)
	{
		sum = 0;
		if (g->fourmis[j].tabou->count >= g->nodelist->count - 1)
		{
			done = true;
			sum = calDist(g->liste_arc, (g->fourmis + j)->tabou);

			if (sum < minSum)
			{
				minSum = sum;
				bestIndex = j;
			}
		}
	}
	//printf("Best index: %d\n", bestIndex);
	if (done == false)
	{
		printf("Error: Aucune fourmis n'a parcourrus tout le graphe\n");
	}
	return bestIndex;
}

ArrayList_t *antColony(Graph g, int gen)
{
	ArrayList_t *shortest;
	ArrayList_t *record = NULL;
	Vector4_t *arrete = NULL;
	int i, j, k;
	double pheromone, d1, d2;
	int total = g->nb_vertices, *nb_passage, nbGeneration = gen;
	nb_passage = malloc(g->liste_arc->count * sizeof *nb_passage);

	for (i = 0; i < g->liste_arc->count; ++i)
	{
		nb_passage[i] = 0;
	}
	//Génération
	for (k = 0; k < nbGeneration; ++k)
	{
		for (i = 0; i < total; ++i)
		{
			for (j = 0; j < total; ++j)
			{
				deplace_fourmis(g, j, nb_passage);
			}
			for (j = 0; j < g->liste_arc->count; ++j)
			{
				arrete = (Vector4_t *)ArrayList_get(g->liste_arc, j);
				pheromone = (arrete->z / 2.0) + 100.0 * nb_passage[j];

				nb_passage[j] = 0;
				arrete->z = pheromone;
			}
		}
		shortest = g->fourmis[get_shortest_path_index(g)].tabou;
		if (record == NULL)
		{
			record = ArrayList_copy(shortest);
			//ArrayList_print(record, ArrayList_printInt);
		}
		else
		{
			//ArrayList_print(record, ArrayList_printInt);
			d1 = calDist(g->liste_arc, shortest);
			d2 = calDist(g->liste_arc, record);
			if (d1 < d2 && shortest->count == g->nb_vertices - 1)
			{
				ArrayList_destroy(record);
				record = ArrayList_copy(shortest);
			}
		}
		//printf("the shortest Path is :");
		//Reset Ant pose
		for (j = 0; j < g->nodelist->count; ++j)
		{
			for (i = 0; i < g->antPerNode; ++i)
			{
				g->fourmis[j * g->antPerNode + i].node = j;
				//g->fourmis[j * g->antPerNode + i].tabou->count = 0;
				while (g->fourmis[j * g->antPerNode + i].tabou->count > 0)
				{
					ArrayList_removeIndex(g->fourmis[j * g->antPerNode + i].tabou, 0);
				}
			}
		}
		//ArrayList_print(shortest, ArrayList_printInt);
	}
	free(nb_passage);
	return record;
}

#ifdef __ANT_COLON_DEBUG
int main()
{
	Vector4_t *arrete;
	Node start, end;
	ArrayList_t *shortest;
	int total = 30;
	Graph g = new_graph(total, 2);
	int i, j = 2;

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

	shortest = antColony(g, 40);
	print_graph(g);
	printf("Meilleur Chemin: ");
	ArrayList_print(shortest, ArrayList_printInt);
	ArrayList_destroy(shortest);
	erase_graph(g);
}
#endif // ! __ANT_COLON_DEBUG