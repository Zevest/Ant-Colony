#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"
#include "arrayList.h"
#include "random.h"
#include "graph.h"

ArrayList_t *chemin_connecte(Graph g , int node, ArrayList_t * tabou){
	int i;
	ArrayList_t * chemins = ArrayList_new(sizeof (Vector4_t));
	//pour recupere tout les chemine connecte au num node
	for (i = 0; i < g->liste_arc->count; ++i)
	{
		
		Vector4_t * branch = (Vector4_t *) ArrayList_get(g->liste_arc,i);
		/*rintf("node: %d i: %d %p\n",node,  i, branch);
		printf("x: %f\n", branch->x);
		printf("y: %f\n", branch->y);*/
		//&& !ArrayList_containValue(tabou, (char*)&node, ArraryList_intComparator)
		//&& !ArrayList_containValue(tabou, (char*)&node, ArraryList_intComparator)
		if(((int)branch->x==node ) || ((int)branch->y==node  ))
		{
			//printf("Added on this_n\n");
			ArrayList_add(chemins,(char*)branch);
		}
	}
	return chemins;
} 
void deplace_fourmis(Graph g , int node){
	int i,j, num_chemin_choisi = 0, nbFourmis = 2;
	ArrayList_t *liste;
	double *liste_prob; 
	Vector4_t *rcp_val;	
	for(j = 0; j < nbFourmis; ++j){
		liste = chemin_connecte(g , node, g->fourmis[node+j].tabou);
		if(liste->count){
		 	liste_prob = malloc(liste->count * sizeof * liste_prob);
		 	
		 	for ( i = 0; i <liste->count; ++i)
		 	{
		 		rcp_val = (Vector4_t *) ArrayList_get(liste,i);
		 		liste_prob[i]=rcp_val->z/pow(rcp_val->w,7);
		 	}
		 	printf("ANT n°%d, %d -> node[%d]\n", j + node*2, node, num_chemin_choisi);
		 	if(0 <= num_chemin_choisi && num_chemin_choisi < liste->count) {
		 		ArrayList_add(g->fourmis[nbFourmis*node + j].tabou, (char *)&node);
		 		num_chemin_choisi = Random_weighted(liste_prob, liste->count);
		 		printf("ant %d moved from %d to %d\n",nbFourmis * node +j,  g->fourmis[nbFourmis*node + j].node, num_chemin_choisi);
		 	
		 		rcp_val = (Vector4_t *) ArrayList_get(liste,num_chemin_choisi);
		 		g->fourmis[nbFourmis*node+j].node = ((int)rcp_val->x == node ? (int) rcp_val->y :  (int) rcp_val->x);
		 	}
		 	//g->fourmis[nbFourmis*node+j].node = num_chemin_choisi;
		 	
		 	free(liste_prob);
		}
		ArrayList_destroy(liste);
	 
	}/*
	if(listeA->count){
	liste_prob = malloc(liste->count * sizeof * liste_prob);

	for ( i = 0; i <liste->count; ++i)
	{
		rcp_val = ArrayList_get(liste,i);
		liste_prob[i]=rcp_val->z/pow(rcp_val->w,7);
	}
	 	
	ArrayList_add(g->fourmis[2*node+1]->tabou, node);
	num_chemin_choisi = Random_weighted(liste_prob, liste->count);
	g->fourmis[2*node+1]->node = num_chemin_choisi;
	free(liste_prob);
	}									
	ArrayList_Destroy(liste);																												
	*/
}
void print_branch(char * b){
	Vector4_t * branch = (Vector4_t *) b;
	printf("node(%.0f -> %.0f)\n len %.3f\n pheromone %.3f\n", branch->x, branch->y, branch->w, branch->z);
	
}

int main(){
	Vector4_t *arrete;
	Node start, end;
	int total = 3;
	Graph g = new_graph(total);
	int i, j = 2;

	for(i = 0; i < total; ++i){
		for(j = i+1; j < total; ++j){
			add_edge(g, i, j);
		}
	}
	
	for (i = 1; i < g->liste_arc->count; i++)
	{
		arrete = (Vector4_t*)ArrayList_get(g->liste_arc, i);
		start = (Node)ArrayList_get(g->nodelist,(int) arrete->x);
		end = (Node)ArrayList_get(g->nodelist,(int) arrete->y);
		arrete->w = Vector2_dist(&start->pos, &end->pos);
	}
	//ArrayList_t * chemin = chemin_connecte(g , 0, g->fourmis[0].tabou);
	for(i = 0; i < 2; ++i){
		for(j = 0; j < total; ++j){
			deplace_fourmis(g, j);
		}
	}
	
	//ArrayList_print(chemin, print_branch);
	//Vector4_t branch = *(Vector4_t *)ArrayList_get(chemin, 0);
	//printf("Arrête(%d, %d)\n	distance: %0.3f\n	phéromone: %0.3f\n", (int)branch.x, (int)branch.y, branch.w, branch.z);
	//free(chemin);
	//print_graph(g);
	erase_graph(g);
}



/*
void ant_colon(graph g, int node){
	
}*/
