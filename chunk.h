#ifndef CHUNK__H
#define CHUNK__H

#include <iostream>
#include "sprite.h"
#include "gamedata.h"

class Chunk : public Sprite {

  public:
    explicit Chunk( const Vector2f& pos, const Vector2f vel, 
                    const string& name, Image* fm) :
      Sprite(name, pos, vel, fm, getAngle(), getRotSpd()), 
      distance(0), 
      maxDistance(Gamedata::getInstance().getXmlInt(name + "/distance")), 
      tooFar(false),
      image(fm)
    { }

    Chunk(const Chunk& )=default;
    Chunk(Chunk&&)=default;

    Chunk& operator=(const Chunk& )=default;
    Chunk& operator=(Chunk&&)=default;

    virtual ~Chunk(){ delete image; }

    bool goneTooFar() const { return tooFar; }
    void reset() {
      tooFar = false;
      distance = 0;
    }

    virtual void update(Uint32 ticks);

  private:
    float distance;
    float maxDistance;
    bool tooFar;
    Image* image;
};
#endif
