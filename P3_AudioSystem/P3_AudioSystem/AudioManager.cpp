#include "AudioManager.h"
#include <string>


// Initialize our static variables
AudioManager* AudioManager::instance = 0;
AudioManager::AudioState AudioManager::currentState = ERROR;


void AudioManager::init()
{
	if (SDL_Init(SDL_INIT_AUDIO) != -1)
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		{
			std::cerr << "Error initializing audio device...\n";
			currentState = ERROR;
		}
		else
		{
			currentState = WAITING;
		}
	}
	else
	{
		std::cerr << "Error initializing SDL audio subsystem...\n";
		currentState = ERROR;
	}

	listenerBlock = new Block(10, 10, 10, 10);
	sourceBlock = new Block(30, 10, 15, 15);



	Raycaster * r = new Raycaster();
	r->setPos(0, 0);
	r->addVertex(0, 0);
	r->addVertex(800, 0);
	r->addVertex(800, 600);
	r->addVertex(0, 600);
	m_Raycaster.push_back(r);


	listener = new Raycaster();
	listener->setPos(listenerBlock->getPos().x, listenerBlock->getPos().y);
	listener->addVertex(0, 0);
	listener->addVertex(0, 10);
	listener->addVertex(10, 10);
	listener->addVertex(10, 0);
	m_Raycaster.push_back(listener);

}

#pragma region Actual Audio functions, play pause stop etc 

void AudioManager::playMusic(const std::string& fileName)
{
	if (currentState != ERROR)
	{
		Mix_Music * music = Mix_LoadMUS(fileName.c_str());
		// If no music is playing, play it
		if (Mix_PlayingMusic() == 0)
		{
			if (music == NULL)
			{
				printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
			}
			//Play music
			Mix_PlayMusic(music, -1);
		}
		else
		{
			// If music is playing, pause it
			this->pauseMusic();
		}
	}
}



void AudioManager::pauseMusic()
{
	if (currentState != ERROR)
	{
		// If music is playing, handle the pause request
		if (Mix_PlayingMusic() == 1)
		{
			if (Mix_PausedMusic() == 1)
			{
				// If we receive a pause request and the music is already paused, resume it.
				Mix_ResumeMusic();
				currentState = PLAYING;
			}
			else
			{
				// Otherwise, pause the music
				Mix_PauseMusic();
				currentState = PAUSED;
			}
		}
	}
}

void AudioManager::stopMusic()
{
	if (currentState != ERROR)
	{
		Mix_HaltMusic();
		currentState = STOPPED;

	}
}

void AudioManager::playFX(const std::string& Filename, bool isDucking,int amount) 
{
	if (currentState != ERROR)
	{
		Mix_Chunk *sound = Mix_LoadWAV(Filename.c_str());

		int SChannel = 0;

		if (sound == NULL)
		{
			Mix_FreeChunk(sound);
			currentState = ERROR;
			printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		}
		else if(attenuation(SChannel, calAngle(), calDistance()))
		{
			SChannel = Mix_PlayChannel(-1, sound, 0);
			if (isDucking) {
				Mix_VolumeMusic(Mix_VolumeMusic(-1) / amount);
			}

			while (Mix_Playing(SChannel) != 0);
			{
				if (isDucking)
				{
					Mix_VolumeMusic(Mix_VolumeMusic(-1) * amount);
				}
				Mix_FreeChunk(sound);
			}
			currentState = PLAYING;
		}
	}
}


void AudioManager::playRaycastFX(const std::string& Filename)
{
	if (currentState != ERROR)
	{
		Mix_Chunk *sound = Mix_LoadWAV(Filename.c_str());

		int SChannel = 0;

		if (sound == NULL)
		{
			Mix_FreeChunk(sound);
			currentState = ERROR;
			printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		}
		else if (dampening()) //if the rays are intercepted by a wall before reaching listener dampen!
		{
			attenuation(SChannel, 0, 254);
			SChannel = Mix_PlayChannel(-1, sound, 0);
			}

			while (Mix_Playing(SChannel) != 0);
			{
				Mix_FreeChunk(sound);
			}
			currentState = PLAYING;
		}
}

bool AudioManager::dampening()
{
	//if (/* if intersection*/)
	//{
	//	return true;
	//}
	return false;
}

void AudioManager::calculateRays(SDL_Point sourcePos)
{
	for (int i = 0; i < m_Raycaster.size(); i++) {
		for (int j = 0; j < m_Raycaster[i]->m_vertices.size(); j++) {
			m_rays.push_back({ sourcePos,{ (m_Raycaster[i]->m_position.x + m_Raycaster[i]->m_vertices[j].x - sourcePos.x), (m_Raycaster[i]->m_position.y + m_Raycaster[i]->m_vertices[j].y - sourcePos.y)  } });
		}
	}
	for (int i = 0; i < m_rays.size(); i++) {
		float minT = std::numeric_limits<float>::max();
		for (int j = 0; j < m_Raycaster.size(); j++) {
			float tempT = m_Raycaster[j]->calMinT(m_rays[i]);
			if (tempT < minT && tempT > 0) {
				minT = tempT;
			}

		}
		m_rays[i].dir.x *= minT;
		m_rays[i].dir.y *= minT;
	}
}


void AudioManager::setVolume(bool volume)
{
	if (volume) {
		// set the music volume to 1/2 maximum, and then check it
		printf("volume was    : %d\n", Mix_VolumeMusic(Mix_VolumeMusic(-1) / 2));
		printf("volume is now : %d\n", Mix_VolumeMusic(-1));

		
	}
	else if (!volume) {

		// set the music volume to 1/2 maximum, and then check it
		printf("volume was    : %d\n", Mix_VolumeMusic(Mix_VolumeMusic(-1) * 2));
		printf("volume is now : %d\n", Mix_VolumeMusic(-1));
		if (Mix_VolumeMusic(-1) == 0)
		{
			Mix_VolumeMusic(Mix_VolumeMusic(-1) + 1);
		}
	}

}
bool AudioManager::attenuation(int channel,int angle,int distance)
{
	if (angle < 0) {
		angle = angle + 360;
	}
	if (distance < 0) {
		distance = 0;
	}
	if (distance > 254)
	{
		return false;
	}

	// set channel 2 to be behind and right, and 100 units away
	if (!Mix_SetPosition(channel, angle, distance)) {
		printf("Mix_SetPosition: %s\n", Mix_GetError());
		// no position effect, is it ok?
	}
	return true;
}

#pragma endregion
int AudioManager::calAngle()
{
	return (atan2(listenerBlock->getPos().y - sourceBlock->getPos().y, listenerBlock->getPos().x - sourceBlock->getPos().x) * 360 / (2 * M_PI));
}

int AudioManager::calDistance()
{
	float y = (listenerBlock->getPos().y - sourceBlock->getPos().y)*(listenerBlock->getPos().y - sourceBlock->getPos().y);
	float x = (listenerBlock->getPos().x - sourceBlock->getPos().x)*(listenerBlock->getPos().x - sourceBlock->getPos().x);
	return sqrt(x + y);
}

#pragma region demoing component functions, creating walls listener& source placement etc
Wall * AudioManager::addWall() {

	Wall * w = new Wall();
	m_walls.push_back(w);
	return w;
}

Raycaster * AudioManager::addRayObject() {
	Raycaster * s = new Raycaster();
	m_Raycaster.push_back(s);
	return s;
}
//void AudioManager::deleteWall(Wall * wall) {
//	m_walls.erase(std::remove());
//	delete wall;
//}
void AudioManager::Update(float x, float y, bool source )
{
	if (source)
	{
		sourceBlock->setPos(x, y);
	}
	else if (!source)
	{
		listenerBlock->setPos(x, y);
	}
	
}
void AudioManager::Update()
{
	listener->setPos(listenerBlock->getPos().x, listenerBlock->getPos().y);
	calculateRays(sourceBlock->getPos());
}

void AudioManager::render(SDL_Renderer &renderer)
{
	for (int i = 0; i < m_walls.size(); i++)
	{
		m_walls.at(i)->render(renderer);
	}
	for (int i = 0; i < m_rays.size(); i++) {
		SDL_SetRenderDrawColor(&renderer, 0, 0, 255, 255);
		SDL_RenderDrawLine(&renderer, m_rays[i].pos.x, m_rays[i].pos.y, m_rays[i].pos.x + m_rays[i].dir.x, m_rays[i].pos.y + m_rays[i].dir.y);
	}
	m_rays.clear();
	listenerBlock->draw(&renderer);
	sourceBlock->draw(&renderer);

}
#pragma endregion