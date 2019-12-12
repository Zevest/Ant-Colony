#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "vector.h"
#include "graph.h"
#include "random.h"

const int SCREEN_WIDTH = 640, SCREEN_HEIGHT = 480;
const char ressourcesPath[] = "rsc";
void logSDLError(FILE * os, const char* msg){
	fprintf(os, "%s error: %s\n", msg, SDL_GetError());
}


/**
* Loads a BMP image into a texture on the rendering device
* @param file The BMP image file to load
* @param renderer The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture * loadTexture(char * file, SDL_Renderer * renderer){
	SDL_Texture * texture = NULL;
	
	SDL_Surface *loadedImage = SDL_LoadBMP(file);
	if(loadedImage != NULL){
		texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
		SDL_FreeSurface(loadedImage);
		
		if(texture ==NULL){
			//fprintf(stdout, "test\n");
			logSDLError(stdout, "CreateTextureFromSurface");
			SDL_Quit();
		}
		
	}else{
		//fprintf(stdout, "test\n");
		logSDLError(stdout, "LoadBMP");
	}
	return texture;
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param texure The source texture we want to draw
* @param renderer The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
*/
void renderTexture(SDL_Texture * texture, SDL_Renderer * renderer, int x, int y){
	SDL_Rect destination;
	destination.x = x;
	destination.y = y;
	SDL_QueryTexture(texture,NULL, NULL, &destination.w, &destination.h);
	SDL_RenderCopy(renderer, texture, NULL, &destination);
}


void circle(SDL_Renderer * renderer, double x, double y, double r){
	int i,j;
	for(j = -r;  j < r; ++j){
		for(i = -r;  i < r; ++i){
			if((i)*(i) + (j)*(j) < r){
				SDL_RenderDrawPoint(renderer,x+i, y+j);
			}
		}
	}
}


void drawGraph(SDL_Renderer * renderer, Graph g){
	int i;
	printf("KARIM STEVE\n");
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	for (i = 0; i < g->liste_arc->count; ++i)
	{
		
		Vector4_t branch = *(Vector4_t *)ArrayList_get(g->liste_arc, i);
		Node start = (Node)ArrayList_get(g->nodelist, (int)branch.x);
		Node end = (Node)ArrayList_get(g->nodelist, (int) branch.y);
		SDL_RenderDrawLine(renderer,
			map(start->pos.x,0, 30, 0, SCREEN_WIDTH),
			map(start->pos.y,0, 30, 0, SCREEN_HEIGHT),
			map(end->pos.x,0, 30, 0, SCREEN_WIDTH),
			map(end->pos.y,0, 30, 0, SCREEN_HEIGHT)
			);
		SDL_RenderDrawLine(renderer,
			map(start->pos.x,0, 30, 0, SCREEN_WIDTH),
			map(start->pos.y,0, 30, 0, SCREEN_HEIGHT),
			map(end->pos.x,0, 30, 0, SCREEN_WIDTH),
			map(end->pos.y,0, 30, 0, SCREEN_HEIGHT)
			);
		SDL_RenderDrawLine(renderer,
			map(start->pos.x,0, 30, 0, SCREEN_WIDTH),
			map(start->pos.y,0, 30, 0, SCREEN_HEIGHT),
			map(end->pos.x,0, 30, 0, SCREEN_WIDTH),
			map(end->pos.y,0, 30, 0, SCREEN_HEIGHT)
			);
		
		//printf("Arrête(%d, %d)\n	distance: %0.3f\n	phéromone: %0.3f\n", (int)branch.x, (int)branch.y, branch.w, branch.z);
	}
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	for (i = 1; i < g->nb_vertices + 1; i++)
	{
		Node n = (Node)ArrayList_get(g->nodelist, i - 1);
		//printf("(%d): ", i);
		if (n != NULL)
		{
			double posX = map(n->pos.x, 0, 30, 0, SCREEN_WIDTH);
			double posY = map(n->pos.y, 0, 30, 0, SCREEN_HEIGHT);
			circle(renderer,posX,posY,50);
		}
		else
			printf("pos(NULL), val; NULL\n");
	}
	printf("FIN FIN\n");
}


/*
 * Lesson 0: Test to make sure SDL is setup properly
 */
int main(int argc, char** argv){
	
	
	
	///////////////////////////////////////////
	Vector4_t *arrete;
	Node start, end;

	int total = 5;
	Graph g = new_graph(total, false);
	int i, j = 2;

	//for (i = 1; i < total; i++)
	//{
		//printf("Doing it for %d times.\n", i);
		//add_edge(g, i, j);
		add_edge(g, 1, 2);
		add_edge(g, 1, 3);
		add_edge(g, 1, 4);
		add_edge(g, 1, 0);
		//add_edge(g, 1, 0);
		//printf("Then\n");
		//j++;
	//}
	
	for (i = 1; i < g->liste_arc->count; i++)
	{
		arrete = (Vector4_t*)ArrayList_get(g->liste_arc, i);
		start = (Node)ArrayList_get(g->nodelist,(int) arrete->x);
		end = (Node)ArrayList_get(g->nodelist,(int) arrete->y);
		arrete->w = Vector2_dist(&start->pos, &end->pos);
		//Vector4_set(arrete, arrete->x, arrete->y, arrete->z, dist);		
		
	}
	//print_graph(g);
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

	

	//////////////////////////////////////////
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		logSDLError(stdout,"SDL_Init");
		return 1;
	}
	SDL_Window *window= SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (window == NULL){
		logSDLError(stdout,"SDL_CreateWindow");
		SDL_Quit();
		return 1;
	}
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(renderer == NULL){
		SDL_DestroyWindow(window);
		logSDLError(stdout,"SDL_CreateRenderer");
		SDL_Quit();
		return 1;
	}
	
	SDL_Rect rec2d = {
		SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 200, 200
	};	
	
	
	//A sleepy rendering loop, wait for 3 seconds and render and present the screen each time
		//First clear the renderer
		//SDL_RenderClear(renderer);
		//Draw the texture
		//SDL_RenderCopy(renderer, texture, NULL, NULL);
		//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		//SDL_RenderFillRect(renderer, &rec2d);
		//SDL_RenderDrawLine(renderer, 0, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
		//circle(renderer, 100, 100, 125); 
		//Update the screen
		//SDL_RenderPresent(renderer);
		//Take a quick break after all that hard work
		//SDL_Delay(500);
	
	SDL_RenderClear(renderer);
	print_graph(g);
	drawGraph(renderer, g);
	SDL_RenderPresent(renderer);
	ArrayList_destroy(g->nodelist);
	ArrayList_destroy(g->liste_arc);
	free(g->fourmis);
	free(g);
		
	SDL_Delay(60000);
	
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	
	SDL_Quit();
	
	
	
	return 0;
}
