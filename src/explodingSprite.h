// This is the "pointers" example in the Brian example

#include <list>
#include <vector>
#include "chunk.h"

class ExplodingSprite : public Sprite {
 public:
  ExplodingSprite(const Sprite& s);
  ~ExplodingSprite();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  unsigned int chunkCount() const { return chunks.size(); }
  unsigned int freeCount() const { return freeList.size(); }
  bool d() { return chunks.empty(); }

 private:
  std::list<Chunk*> chunks;    // An ExplodingSprite is a list of sprite chunks
  std::list<Chunk*> freeList;  // When a chunk gets out of range it goes here
  void makeChunks(unsigned int);

  ExplodingSprite(const ExplodingSprite&);             // Explicit disallow (Item 6)
  ExplodingSprite& operator=(const ExplodingSprite&);  // (Item 6)
};
