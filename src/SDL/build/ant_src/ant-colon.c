#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"
#include "arrayList.h"
#include "random.h"
#include "graph.h"
//#define __ANT_COLON_DEBUG

/**
 * Affiche une branche du graphe 
 * @param b address d'un vecteur4_t caster en (char *)
 */
void print_branch(char *b)
{
	Vector4_t *branch = (Vector4_t *)b;
	printf("node(%.0f -> %.0f) len %.3f pheromone %.3f\n",
		   branch->x, branch->y, branch->w, branch->z);
}

void printPath(Graph g, ArrayList_t *path);

/**
 * Comparateur de branches d'un graphe
 * @param a addresse d'un branch d'un graphe caster en (char *)
 * @param b addresse d'une branche d'un graphe caster en (char *)
 * @return 0 si les branches ont le même point de départ et d'arrivé
 */
int BranchCompar(char *a, char *b)
{
	Vector4_t *branchA = (Vector4_t *)a;
	Vector4_t *branchB = (Vector4_t *)b;
	//if((int)a->x == (int)b->x && (int)a-<y == (int)b->y)
	return (int)branchA->x - (int)branchB->x + (int)branchA->y - (int)branchB->y;
}

/**
 * Fabrique une liste de toutes les branches d'un graphe connectées au noeud d'indice donné
 * @param g Le graphe concerné
 * @param nodeNum L'indice du Noeud dans le graph
 * @param tabou La liste des noeud a ignorer
 * @return Une ArrayList contenant l'indice des branches connectées
 */
ArrayList_t *chemin_connecte(Graph g, int nodeNum, ArrayList_t *tabou)
{
	int i, tmpA, tmpB;
	ArrayList_t *connectedPath = ArrayList_new(sizeof(Vector4_t));
	for (i = 0; i < g->liste_arc->count; ++i)
	{
		Vector4_t *branch = (Vector4_t *)ArrayList_get(g->liste_arc, i);
		tmpA = (int)branch->x;
		tmpB = (int)branch->y;
		if (tmpA == nodeNum &&
			!ArrayList_containValue(tabou, (char *)&tmpB, ArrayList_intComparator))
		{
			ArrayList_add(connectedPath, (char *)branch);
		}
		else if (tmpB == nodeNum &&
				 !ArrayList_containValue(tabou, (char *)&tmpA, ArrayList_intComparator))
		{
			ArrayList_add(connectedPath, (char *)branch);
		}
	}
	return connectedPath;
}
/**
 * Déplace les fourmis présentes sur le noeud d'indice donné dans un graphe 
 * Compt egalement le nombre de fourmis passant par les branches connecter au noeud
 * @param g Le graphe conerné
 * @param nodeNum L'indice du noeud
 * @param count Liste du compte des branches empruntées par les fourmis
 */
void deplace_fourmis(Graph g, int nodeNum, int *count)
{
	int j, i, choosed_number = 0, antNum = g->antPerNode * nodeNum, oldNode, index = 0;
	ArrayList_t *validPath;
	Vector4_t *tmp = NULL;
	double *weights = NULL;
	weights = malloc(g->nodelist->count * sizeof *weights);
	for (i = 0; i < g->nodelist->count; weights[i++] = 0)
		;
	for (j = 0; j < g->antPerNode; ++j)
	{
		validPath = chemin_connecte(g, g->fourmis[antNum + j].node,
									(g->fourmis + antNum + j)->tabou);
		if (validPath->count > 0) // si il existe un chemin
		{
			for (i = 0; i < validPath->count; ++i)
			{
				tmp = (Vector4_t *)ArrayList_get(validPath, i);
				double nWeight = tmp->z / pow(tmp->w, 7) * 1000;
				weights[i] = nWeight;
			}
			choosed_number = Random_weighted(weights, validPath->count);
			//printf("%d\n", choosed_number);
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
		}
		ArrayList_destroy(validPath);
	}
	free(weights);
}
/**
 * Calcule la distance total du chemin parcouru par une fourmis
 * @param branch addresse de l 'ArrayListe de toutes les branches
 * @param path addresse de l'ArrayList contenant les noeuds traverser pas la fourmis
 * @return La somme des distance entre les noeuds
 */
double calDist(ArrayList_t *branch, ArrayList_t *path)
{
	int *pathPoints, i, index;
	Vector4_t *tmp = NULL;
	double sum = 0;
	for (i = 0, pathPoints = (int *)ArrayList_get(path, 0); i < path->count - 1; ++pathPoints, ++i)
	{
		Vector4_t nPath = {*pathPoints, *(pathPoints + 1), 0, 0};
		index = ArrayList_indexOf(branch, (char *)&nPath, BranchCompar);
		tmp = (Vector4_t *)ArrayList_get(branch, index);
		sum += tmp->w;
	}
	return sum;
}
/**
 * Trouve l'indice du chemin le plus cours parmis ceux emprunter par les fourmis
 * @param g le graphe concerné
 * @return l'indice de la fourmis ayant parcouru la plus courte distance
 */
int get_shortest_path_index(Graph g)
{
	int bestIndex = 0, i;
	Bool done = false;
	double minSum = INFINITY, sum;
	for (i = 0; i < g->nb_vertices * g->antPerNode; ++i)
	{
		sum = 0;
		if (g->fourmis[i].tabou->count >= g->nodelist->count - 1)
		{
			done = true;
			sum = calDist(g->liste_arc, (g->fourmis + i)->tabou);
			if (sum < minSum)
			{
				minSum = sum;
				bestIndex = i;
			}
		}
	}
	if (done == false)
	{
		fprintf(stderr, "Error: Aucune fourmis n'a parcourrus tout le graphe\n");
	}
	return bestIndex;
}
/**
 * Affiche une liste de noeud sous forme de chemin pour déboguer
 * @param g le graphe concerné
 * @param path La liste des noeuds du chemin
 */
void printPath(Graph g, ArrayList_t *path)
{
	int i;
	int *tmp = NULL, index;
	Vector4_t branch, nPath;
	for (i = 0, tmp = (int *)path->data; i < path->count - 1; ++tmp, ++i)
	{
		//ArrayList_print(path, ArrayList_printInt);
		nPath.x = *tmp;
		nPath.y = *(tmp + 1);
		index = ArrayList_indexOf(g->liste_arc, (char *)&nPath, BranchCompar);
		branch = *(Vector4_t *)ArrayList_get(g->liste_arc, index);
		//start = (Node)ArrayList_get(g->nodelist, (int)branch.x);
		//end = (Node)ArrayList_get(g->nodelist, (int)branch.y);
		printf("Path %d from %d to %d\n", index, (int)branch.x, (int)branch.y);
	}
}

/**
 * Trouve le chemin le plus court passant par tous les noeuds d'un graphe
 * @param g le graphe concerné
 * @param gen le nombre génération (d'iteration) de fourmis
 * @return la liste des noeuds dans l'ordre constituant le chemin le plus cours
 */
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
		}
		else
		{
			d1 = calDist(g->liste_arc, shortest);
			d2 = calDist(g->liste_arc, record);
			if (d1 < d2 && shortest->count == g->nb_vertices - 1)
			{
				ArrayList_destroy(record);
				record = ArrayList_copy(shortest);
			}
		}
		//Rénitialise la position des fourmis
		for (j = 0; j < g->nodelist->count; ++j)
		{
			for (i = 0; i < g->antPerNode; ++i)
			{
				g->fourmis[j * g->antPerNode + i].node = j;
				while (g->fourmis[j * g->antPerNode + i].tabou->count > 0)
				{
					ArrayList_removeIndex(g->fourmis[j * g->antPerNode + i].tabou, 0);
				}
			}
		}
	}
	free(nb_passage);
	return record;
}
/**
 * Fonction Debogging pour tester l'algorithme sans l'affichage SDL
 * @param g le graph concerné
 * @param path le chemin
 */
void testSDLwWithout(Graph g, ArrayList_t *path)
{
	int i, *tmp = NULL, *pathPoints = NULL;
	Node start = NULL, end = NULL;
	//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	for (i = 0, pathPoints = (int *)path->data; i < path->count; ++pathPoints, ++i)
	{
		if (i == path->count - 1)
		{
			printf("(%d, %d)", *pathPoints, *(int *)path->data);
			start = (Node)ArrayList_get(g->nodelist, *pathPoints);
			end = (Node)ArrayList_get(g->nodelist, *(int *)path->data);
		}
		else
		{
			printf("(%d, %d)", *pathPoints, *(pathPoints + 1));
			start = (Node)ArrayList_get(g->nodelist, *pathPoints);
			end = (Node)ArrayList_get(g->nodelist, *(pathPoints + 1));
		}

		/*SDL_RenderDrawLine(renderer,
							   map(start->pos.x, 0, 30, 0, SCREEN_WIDTH),
							   map(start->pos.y, 0, 30, 0, SCREEN_HEIGHT),
							   map(end->pos.x, 0, 30, 0, SCREEN_WIDTH),
							   map(end->pos.y, 0, 30, 0, SCREEN_HEIGHT));*/
	}
	printf("\n");
}

#ifdef __ANT_COLON_DEBUG
int main()
{
	Vector4_t *arrete;
	Node start, end;
	ArrayList_t *shortest;
	int total = 30;
	Graph g = new_graph(total, 4);
	int i, j;

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
	int *tmp = NULL;
	Vector4_t branch;
	testSDLwWithout(g, shortest);

	printf("Meilleur Chemin: ");

	ArrayList_print(shortest, ArrayList_printInt);
	ArrayList_destroy(shortest);
	erase_graph(g);
}
#endif // ! __ANT_COLON_DEBUG