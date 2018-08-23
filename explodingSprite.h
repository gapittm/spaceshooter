#include <vector>
#include <list>
#include "chunk.h"

class ExplodingSprite : public Sprite {

  public:
    ExplodingSprite(const Sprite& s);
    ~ExplodingSprite();

    void makeChunks(unsigned int);
    unsigned int chunkCount() const { return chunks.size(); }
    unsigned int freeCount()  const { return freeList.size(); }

    virtual void draw() const;
    virtual void update(Uint32 ticks);

  private:
    std::list<Chunk*> chunks;   // Active chunks
    std::list<Chunk*> freeList; // Expired chunks

    ExplodingSprite(const ExplodingSprite&);
    ExplodingSprite& operator=(const ExplodingSprite&);
};
