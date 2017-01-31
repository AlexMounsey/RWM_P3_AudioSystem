#ifndef _SOUNDMANAGER_H
#define	_SOUNDMANAGER_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include <iostream>
#include <cmath>
#include "Wall.h"
#include "Block.h"
#include "Raycaster.h"


	class AudioManager
	{
	private:

		static AudioManager* instance;
		

		enum AudioState
		{
			ERROR = 0,
			WAITING,
			PAUSED,
			STOPPED,
			PLAYING
		};
		static AudioState currentState;
		std::vector<Wall *> m_walls;
		std::vector<Ray> m_rays;
		std::vector<Raycaster *> m_Raycaster;
		Block *sourceBlock;
		Block *listenerBlock;
		Raycaster * listener;

		AudioManager & operator=(const AudioManager&)
		{
		}
	public:

		static AudioManager* getInstance()
		{
			if (instance == 0)
			{
				instance = new AudioManager;
			}
			return instance;
		}

		void init();

		void playMusic(const std::string & fileName);
		void calculateRays(SDL_Point sourcePos);
		void pauseMusic();
		void stopMusic();
		void playFX(const std::string & Filename, bool isDucking, int amount) ;

		void playRaycastFX(const std::string & Filename);

		bool dampening();

		void setVolume(bool volume);

		bool attenuation(int channel, int angle, int distance);

		int calAngle();

		int calDistance();


		Wall * addWall();

		Raycaster * addRayObject();


		void Update(float x, float y, bool source);

		void Update();

		void render(SDL_Renderer &renderer);

	

	};

#endif

