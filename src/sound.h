#ifndef _SOUND_H
#define _SOUND_H

#include <SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>

class SDLSound {
 public:
  SDLSound();
  ~SDLSound();

  void startMusic();
  void stopMusic();          // stop all sounds
  void toggleMusic() const;  // toggle music on/off
  void operator[](int);      // play the indexed sound
 private:
  int volume;
  int currentSound;
  Mix_Music* music;

  int audioRate;
  int audioChannels;
  int audioBuffers;
  std::vector<Mix_Chunk*> sounds;
  std::vector<int> channels;
  SDLSound(const SDLSound&);
  SDLSound& operator=(const SDLSound&);
};

#endif
