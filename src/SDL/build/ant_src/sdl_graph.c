#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "vector.h"
#include "graph.h"

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


void circle(SDL_Renderer * renderer, int x, int y, int r){
	int i,j;
	for(j = 0;  j < r; ++j){
		for(i = 0;  i < r; ++i){
			if((i-x)*(i-x) + (j-y)*(j-y) < r){
				SDL_RenderDrawPoint(renderer,x+i, y+j);
			}
		}
	}
}

/*
 * Lesson 0: Test to make sure SDL is setup properly
 */
int main(int argc, char** argv){
	
	
	
	///////////////////////////////////////////
	Vector4_t *arrete;
	Node start, end;


	Graph g = new_graph(30, false);
	int i, j = 2;
	for (i = 1; i < 30; i++)
	{
		//printf("Doing it for %d times.\n", i);
		add_edge(g, i, j);
		//printf("Then\n");
		j++;
	}
	
	for (i = 1; i < 30; i++)
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
	for (int i = 0; i < 3; ++i){
		//First clear the renderer
		SDL_RenderClear(renderer);
		//Draw the texture
		//SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &rec2d);
		SDL_RenderDrawLine(renderer, 0, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
		circle(renderer, 100, 100, 125); 
		//Update the screen
		SDL_RenderPresent(renderer);
		//Take a quick break after all that hard work
		SDL_Delay(500);
	}
	
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	
	SDL_Quit();
	
	return 0;
}
