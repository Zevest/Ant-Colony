#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "vector.h"
#include "graph.h"
#include "random.h"
#include "ant-colon.h"

const int SCREEN_WIDTH = 1280, SCREEN_HEIGHT = 720;
const char ressourcesPath[] = "rsc";
void logSDLError(FILE *os, const char *msg)
{
	fprintf(os, "%s error: %s\n", msg, SDL_GetError());
}

/**
* Loads a BMP image into a texture on the rendering device
* @param file The BMP image file to load
* @param renderer The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture *loadTexture(char *file, SDL_Renderer *renderer)
{
	SDL_Texture *texture = NULL;

	SDL_Surface *loadedImage = SDL_LoadBMP(file);
	if (loadedImage != NULL)
	{
		texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
		SDL_FreeSurface(loadedImage);

		if (texture == NULL)
		{
			//fprintf(stdout, "test\n");
			logSDLError(stdout, "CreateTextureFromSurface");
			SDL_Quit();
		}
	}
	else
	{
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
void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y)
{
	SDL_Rect destination;
	destination.x = x;
	destination.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &destination.w, &destination.h);
	SDL_RenderCopy(renderer, texture, NULL, &destination);
}

void circle(SDL_Renderer *renderer, double x, double y, double r)
{
	int i, j;
	for (j = -r; j < r; ++j)
	{
		for (i = -r; i < r; ++i)
		{
			if ((i) * (i) + (j) * (j) < r)
			{
				SDL_RenderDrawPoint(renderer, x + i, y + j);
			}
		}
	}
}

void drawBestGraphPath(SDL_Renderer *renderer, Graph g, ArrayList_t *path)
{

	int i, *tmp = NULL, *pathPoints = NULL, index = 0;
	double posX, posY;
	Vector4_t branch;
	Node start = NULL, end = NULL;
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	/*for (i = 0, pathPoints = (int *)shortest->data; i < shortest->count - 1; ++pathPoints, ++i)
	{
		Vector4_t nPath = {*pathPoints, *(pathPoints + 1), 0, 0};
		index = ArrayList_indexOf(g->liste_arc, (char *)&nPath, BranchCompar);
		printf("[%d](%d, %d) ", index, (int)nPath.x, (int)nPath.y);
	}*/

	for (i = 0, pathPoints = (int *)path->data; i < path->count; ++pathPoints, ++i)
	{
		Vector4_t nPath = {*pathPoints, *(pathPoints + 1), 0, 0};
		index = ArrayList_indexOf(g->liste_arc, (char *)&nPath, BranchCompar);
		tmp = (int *)ArrayList_get(path, index);
		branch = *(Vector4_t *)ArrayList_get(g->liste_arc, *tmp);
		start = (Node)ArrayList_get(g->nodelist, (int)branch.x);
		end = (Node)ArrayList_get(g->nodelist, (int)branch.y);
		SDL_RenderDrawLine(renderer,
						   map(start->pos.x, 0, 30, 0, SCREEN_WIDTH),
						   map(start->pos.y, 0, 30, 0, SCREEN_HEIGHT),
						   map(end->pos.x, 0, 30, 0, SCREEN_WIDTH),
						   map(end->pos.y, 0, 30, 0, SCREEN_HEIGHT));
	}

	for (i = 1; i < g->nb_vertices + 1; i++)
	{
		start = (Node)ArrayList_get(g->nodelist, i - 1);
		//printf("(%d): ", i);
		if (start != NULL)
		{
			posX = map(start->pos.x, 0, 30, 0, SCREEN_WIDTH);
			posY = map(start->pos.y, 0, 30, 0, SCREEN_HEIGHT);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			circle(renderer, posX, posY, 80);
			SDL_SetRenderDrawColor(renderer, 0, 175, 255, 255);
			circle(renderer, posX, posY, 50);
		}
		else
			printf("pos(NULL), val; NULL\n");
	}
}

void drawGraph(SDL_Renderer *renderer, Graph g)
{
	int i;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	for (i = 0; i < g->liste_arc->count; ++i)
	{

		Vector4_t branch = *(Vector4_t *)ArrayList_get(g->liste_arc, i);
		Node start = (Node)ArrayList_get(g->nodelist, (int)branch.x);
		Node end = (Node)ArrayList_get(g->nodelist, (int)branch.y);
		SDL_RenderDrawLine(renderer,
						   map(start->pos.x, 0, 30, 0, SCREEN_WIDTH),
						   map(start->pos.y, 0, 30, 0, SCREEN_HEIGHT),
						   map(end->pos.x, 0, 30, 0, SCREEN_WIDTH),
						   map(end->pos.y, 0, 30, 0, SCREEN_HEIGHT));
		//printf("Arrête(%d, %d)\n	distance: %0.3f\n	phéromone: %0.3f\n", (int)branch.x, (int)branch.y, branch.w, branch.z);
	}

	for (i = 1; i < g->nb_vertices + 1; i++)
	{
		Node n = (Node)ArrayList_get(g->nodelist, i - 1);
		//printf("(%d): ", i);
		if (n != NULL)
		{
			double posX = map(n->pos.x, 0, 30, 0, SCREEN_WIDTH);
			double posY = map(n->pos.y, 0, 30, 0, SCREEN_HEIGHT);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			circle(renderer, posX, posY, 80);
			SDL_SetRenderDrawColor(renderer, 0, 175, 255, 255);
			circle(renderer, posX, posY, 50);
		}
		else
			printf("pos(NULL), val; NULL\n");
	}
}

/*
 * Lesson 0: Test to make sure SDL is setup properly
 */
int main(int argc, char **argv)
{

	///////////////////////////////////////////
	Vector4_t *arrete;
	Node start, end;
	ArrayList_t *shortest;
	int total = 10;
	Graph g = new_graph(total, 4);
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

	//////////////////////////////////////////

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		logSDLError(stdout, "SDL_Init");
		return 1;
	}
	SDL_Window *window = SDL_CreateWindow("Ant Colony", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		logSDLError(stdout, "SDL_CreateWindow");
		SDL_Quit();
		return 1;
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		SDL_DestroyWindow(window);
		logSDLError(stdout, "SDL_CreateRenderer");
		SDL_Quit();
		return 1;
	}

	SDL_Rect rec2d = {
		SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 200, 200};

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

	print_graph(g);

	SDL_Event event;

	Bool exit = false;
	while (!exit)
	{
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
		{
			SDL_Log("Program quit after %i ticks", event.quit.timestamp);
			exit = true;
			break;
		}
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
		drawGraph(renderer, g);
		drawBestGraphPath(renderer, g, shortest);
		SDL_RenderPresent(renderer);
	}

	erase_graph(g);
	ArrayList_destroy(shortest);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
