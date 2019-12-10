#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

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
	/*
	SDL_Texture *background = loadTexture("background.bmp", renderer),
				*foreground = loadTexture("foreground.bmp", renderer);
	
	
	if(background == NULL || foreground == NULL){
		SDL_DestroyTexture(background);
		SDL_DestroyTexture(foreground);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		//cleanup(background, foreground, renderer, window);
		//printf(stdout, "test\n");
		SDL_Quit();
		return 1;
	}
	
	
	//fprintf(stdout, "test\n");
	//SDL_RenderClear(renderer);
	int bw, bh;
	SDL_QueryTexture(background, NULL, NULL, &bw, &bh);
	renderTexture(background, renderer, 0, 0);
	renderTexture(background, renderer, bw, 0);
	renderTexture(background, renderer, 0, bh);
	renderTexture(background, renderer, bw, bh);
	fprintf(stdout, "test\n");
	int fw, fh;
	SDL_QueryTexture(foreground, NULL, NULL, &fw, &fh);
	int fx = SCREEN_WIDTH/2 - fw/2;
	int fy = SCREEN_HEIGHT/2 - fh/2;
	printf("%d %d %d %d %d %d\n", bw, bh, fx, fy, fw, fh);
	renderTexture(foreground, renderer, fx, fy);
	SDL_RenderPresent(renderer);
	//SDL_Delay(2000);
	//int SDL_SetRenderDrawColor(SDL_Renderer* renderer,255, 0, 0, 0);

	SDL_Rect rec2d = {
		fx, fy, 200, 200
	};	
	
	char * image_path = "test.bmp";
	SDL_Surface *bmp = SDL_LoadBMP(image_path);
	if(bmp == NULL){
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
		SDL_Quit();
	}
	
	SDL_Texture * texture  = SDL_CreateTextureFromSurface(renderer, bmp);
	SDL_FreeSurface(bmp);
	if(texture == NULL){
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
		SDL_Quit();
	}
	*/
	//A sleepy rendering loop, wait for 3 seconds and render and present the screen each time
	for (int i = 0; i < 3; ++i){
		//First clear the renderer
		SDL_RenderClear(renderer);
		//Draw the texture
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &rec2d);
		SDL_RenderDrawLine(renderer, 0, 0, fx, fy);
		circle(renderer, 100, 100, 125); 
		//Update the screen
		SDL_RenderPresent(renderer);
		//Take a quick break after all that hard work
		SDL_Delay(500);
	}
	
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	
	SDL_Quit();
	return 0;
}
