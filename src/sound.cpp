#include "sound.h"
#include <ctime>
#include <iostream>
#include <string>
using std::string;

SDLSound::~SDLSound() {
  std::cout << "Cleaning up sounds ..." << std::endl;
  std::clock_t start = std::clock();
  Mix_HaltMusic();
  Mix_FreeMusic(music);
  for (unsigned int i = 0; i < sounds.size(); ++i) {
    Mix_FreeChunk(sounds[i]);
  }
  Mix_CloseAudio();
  std::clock_t duration = std::clock() - start;
  std::cout << "Clean up took " << duration << " ticks\n";
}

SDLSound::SDLSound()
    :  // volume(SDL_MIX_MAXVOLUME/10),
      volume(SDL_MIX_MAXVOLUME / 7),
      currentSound(-1),
      music(NULL),
      audioRate(22050),
      audioChannels(2),
      audioBuffers(4096),
      sounds(),
      channels() {
  if (Mix_OpenAudio(audioRate, MIX_DEFAULT_FORMAT, audioChannels, audioBuffers)) {
    throw string("Unable to open audio!");
  }
  music = Mix_LoadMUS("res/sound/bgm.mp3");
  if (!music) throw string("Couldn't load BGM.mp3") + Mix_GetError();

  startMusic();

  sounds.push_back(Mix_LoadWAV("res/sound/boom1.wav"));
  sounds.push_back(Mix_LoadWAV("res/sound/boom_gun.wav"));
  sounds.push_back(Mix_LoadWAV("res/sound/boom4.wav"));
  sounds.push_back(Mix_LoadWAV("res/sound/sound4.wav"));
  sounds.push_back(Mix_LoadWAV("res/sound/sound5.wav"));
  sounds.push_back(Mix_LoadWAV("res/sound/explosion.wav"));
  sounds.push_back(Mix_LoadWAV("res/sound/cannon.wav"));
  sounds.push_back(Mix_LoadWAV("res/sound/sound6.wav"));
  sounds.push_back(Mix_LoadWAV("res/sound/sound7.wav"));
  for (unsigned int i = 0; i < sounds.size(); ++i) channels.push_back(i);
  std::cout << "Loaded Sound and Music." << std::endl;
}

void SDLSound::toggleMusic() const {
  if (Mix_PausedMusic()) {
    Mix_ResumeMusic();
  } else {
    Mix_PauseMusic();
  }
}

void SDLSound::operator[](int index) {
  if (currentSound >= 0) Mix_HaltChannel(currentSound);
  currentSound = index;
  Mix_VolumeChunk(sounds[index], volume);
  channels[index] = Mix_PlayChannel(-1, sounds[index], 0);
}

void SDLSound::startMusic() {
  Mix_VolumeMusic(volume);
  Mix_PlayMusic(music, -1);
}

void SDLSound::stopMusic() {
  Mix_HaltMusic();
  Mix_FreeMusic(music);
}
