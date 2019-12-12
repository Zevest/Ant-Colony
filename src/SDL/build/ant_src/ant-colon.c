#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"
#include "arrayList.h"
#include "random.h"
#include "graph.h"

ArrayList_t *chemin_connecte(graph g , int node){
	int i;
	ArrayList_t * chemins;
	//pour recupere tout les chemine connecte au num node 
	for (i = 0; i < g->liste_arc->count; ++i)
	{
		vector4_t branch = *(vector4_t *) Arraylist_get(g->liste_arc,i);
		if(branch.x==node || branch.y==node){
			ArrayList_add(g->liste_arc,(char*)&branch);
		}

	}
return chemins;
} 
void deplace_fourmis(graph g , int node){
	 ArrayList_t *liste = chemins_connecte(g , node);
	 int i,num_chemin_choisi;
	 double *liste_prob; 
	 vector4_t *rcp_val;
	 if(liste->count){
	 	liste_prob = malloc(liste->count * sizeof * liste_prob);
	 	for ( i = 0; i <liste->count; ++i)
	 	{
	 		rcp_val = ArrayList_get(liste,i);
	 		liste_prob[i]=rcp_val->z/pow(rcp_val->w,7);
	 	}
	 	num_chemin_choisi = Random_weighted(liste_prob,liste->count);

	 }																																								

}
void ant_colon(graph g, int node){

}