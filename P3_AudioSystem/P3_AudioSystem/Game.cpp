#include "Game.h"
#include <iostream>
#include <time.h>
Game::Game()
{
}

bool Game::initSDL(int width, int height, const char* title) {

	int e = SDL_Init(SDL_INIT_EVERYTHING);

	srand(time(0));

	if (e != 0)
	{
		// problem with SDL?...
		std::cout << "Could not init SDL: " << SDL_GetError() << std::endl;
		return false;
	}

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width,
		height,
		SDL_WINDOW_OPENGL
	);

	// Check that the window was successfully created
	if (window == NULL)
	{
		// In the case that the window could not be made...
		std::cout << "Could not create window: " << SDL_GetError() << std::endl;
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		// In the case that the renderer could not be made...
		std::cout << "Could not create renderer: " << SDL_GetError() << std::endl;
		return false;
	}

	sr1 = { 500, 300, 100, 10 };
	sr2 = { 300, 200, 100, 10 };
	sr3 = { 500, 200, 10, 100 };
	sr4 = { 600, 200, 10, 100};
	sr5 = { 500, 300, 100, 10 };

	AudioManager::getInstance()->init();
	//AudioManager::getInstance()->playMusic("audio/BackgroundMusic.wav");

	w1 = AudioManager::getInstance()->addWall();
	w1->setRect(sr1);
	w2 = AudioManager::getInstance()->addWall();
	w2->setRect(sr2);
	w3 = AudioManager::getInstance()->addWall();
	w3->setRect(sr3);
	w4 = AudioManager::getInstance()->addWall();
	w4->setRect(sr4);

	s1 = AudioManager::getInstance()->addRayObject();
	s1->setPos(sr2.x, sr2.y);
	s1->addVertex(0, 0);
	s1->addVertex(100, 0);
	s1->addVertex(100, 10);
	s1->addVertex(10, 0);

	s2 = AudioManager::getInstance()->addRayObject();
	s2->setPos(sr3.x, sr3.y);
	s2->addVertex(0, 0);
	s2->addVertex(0, 10);
	s2->addVertex(10, 100);
	s2->addVertex(0, 100);

	s3 = AudioManager::getInstance()->addRayObject();
	s3->setPos(sr4.x, sr4.y);
	s3->addVertex(0, 0);
	s3->addVertex(0, 10);
	s3->addVertex(10, 100);
	s3->addVertex(0, 100);

	s4 = AudioManager::getInstance()->addRayObject();
	s4->setPos(sr1.x, sr1.y);
	s4->addVertex(0, 0);
	s4->addVertex(100, 0);
	s4->addVertex(100, 10);
	s4->addVertex(10, 0);




	return true;
}

void Game::loop()
{

	while (true) {
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		AudioManager::getInstance()->Update();
		AudioManager::getInstance()->render(*renderer);

		KeyboardEvents();

		SDL_RenderPresent(renderer);
	}
}


void Game::KeyboardEvents()
{

	SDL_Event eventV;
	while (SDL_PollEvent(&eventV)) {
			if (eventV.type == SDL_KEYDOWN) {

				/* Check the SDLKey values and move change the coords */
				switch (eventV.key.keysym.sym) {
				case SDLK_SPACE:
					AudioManager::getInstance()->pauseMusic();
					break;
				case SDLK_a:
					AudioManager::getInstance()->setVolume(true);
					break;
				case SDLK_d:
					AudioManager::getInstance()->setVolume(false);
					break;
				case SDLK_p:
					AudioManager::getInstance()->playFX("audio/explosion.wav",true,2);
					break;
				case SDLK_o:
					AudioManager::getInstance()->playMusic("audio/BackgroundMusic.wav");
					break;
				case SDLK_x:
					AudioManager::getInstance()->stopMusic();
					break;
				case SDLK_r:
					AudioManager::getInstance()->playRaycastFX("audio/explosion.wav");
					break;

				}
		}

			else if (eventV.type == SDL_MOUSEBUTTONDOWN) {

				if (eventV.button.button == SDL_BUTTON_LEFT && !sourcePlaced)
				{

					float x = eventV.button.x;
					float y = eventV.button.y;
					AudioManager::getInstance()->Update(x, y, true);
				}

				else if (eventV.button.button == SDL_BUTTON_RIGHT && !listenerPlaced)
				{
					float x = eventV.button.x;
					float y = eventV.button.y;
					AudioManager::getInstance()->Update(x, y, false);

				}
				else if (eventV.button.button == SDL_BUTTON_LEFT &&sourcePlaced &&listenerPlaced)
				{
					sourcePlaced = false;
				}
				else if (eventV.button.button == SDL_BUTTON_RIGHT &&sourcePlaced &&listenerPlaced)
				{
					listenerPlaced = false;

				}
			}
	}




}
