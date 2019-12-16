#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "vector.h"
#include "graph.h"
#include "random.h"
#include "ant-colon.h"

const int SCREEN_WIDTH = 1280, SCREEN_HEIGHT = 720;

void logSDLError(FILE *os, const char *msg)
{
	fprintf(os, "%s error: %s\n", msg, SDL_GetError());
}

/**
 * Dessine un cercle Avec SDL
 * @param renderer permet l'affichage
 * @param x position x du cercle
 * @param y position y du cercle
 * @param r Rayon du cercle
 */
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

/**
 * Affiche le chemin le plus court passant par tous les noeuds d'un graphe
 * @param renderer permet l'affichage
 * @param g Graph a afficher
 */
void drawBestGraphPath(SDL_Renderer *renderer, Graph g, ArrayList_t *path)
{

	int i, *tmp = NULL, *pathPoints = NULL, index = 0;
	double posX, posY;
	Node start = NULL, end = NULL;
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	for (i = 0, pathPoints = (int *)path->data; i < path->count; ++pathPoints, ++i)
	{
		if (i == path->count - 1)
		{
			//printf("(%d, %d)", *pathPoints, *(int *)path->data);
			start = (Node)ArrayList_get(g->nodelist, *pathPoints);
			end = (Node)ArrayList_get(g->nodelist, *(int *)path->data);
		}
		else
		{
			//printf("(%d, %d)", *pathPoints, *(pathPoints + 1));
			start = (Node)ArrayList_get(g->nodelist, *pathPoints);
			end = (Node)ArrayList_get(g->nodelist, *(pathPoints + 1));
		}

		SDL_RenderDrawLine(renderer,
						   map(start->pos.x, 0, 30, 0, SCREEN_WIDTH),
						   map(start->pos.y, 0, 30, 0, SCREEN_HEIGHT),
						   map(end->pos.x, 0, 30, 0, SCREEN_WIDTH),
						   map(end->pos.y, 0, 30, 0, SCREEN_HEIGHT));
	}

	for (i = 0; i < g->nb_vertices; i++)
	{
		start = (Node)ArrayList_get(g->nodelist, i);
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

/**
 * Affiche une graph avec SDL
 * @param renderer permet l'affichage
 * @param g Graph a afficher
 */
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
	}

	for (i = 1; i < g->nb_vertices + 1; i++)
	{
		Node n = (Node)ArrayList_get(g->nodelist, i - 1);
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
