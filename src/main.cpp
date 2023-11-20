#include <iostream>
#include "game.h"

int main(int argc, char** args) 
{
	Game engine;
	if (engine.Initialize()) {
		engine.RunLoop();
	}

	engine.Shutdown();

	return 0;
	//SDL_Surface* winSurface = NULL;
	//SDL_Window* window = NULL;

	

	//window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
	//if (!window) {
	//	std::cout << "Error creating window: " << SDL_GetError() << std::endl;
	//	system("pause");
	//	// End the program
	//	return 1;
	//}

	//winSurface = SDL_GetWindowSurface(window);
	//if (!winSurface) {
	//	std::cout << "Error getting surface: " << SDL_GetError() << std::endl;
	//	system("pause");
	//	// End the program
	//	return 1;
	//}

	//// Fill the window with a white rectangle
	//SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 255, 255, 255));
	//// Update the window display
	//SDL_UpdateWindowSurface(window);
	
}
