#ifndef SPRITE__H
#define SPRITE__H

#include <string>
#include "drawable.h"

class ExplodingSprite;

class Sprite : public Drawable {

  public:
    Sprite(const std::string&);
    Sprite(const std::string&, const Vector2f&);
    Sprite(const std::string&, const Vector2f&, const Vector2f&, 
           const Image*, const float, const float);
    Sprite(const Sprite&);
    virtual ~Sprite() { }
    Sprite& operator=(const Sprite&);

    virtual void draw() const;
    virtual void update(Uint32 ticks);

    virtual const Image* getImage() const { return image; }
    virtual const SDL_Surface* getSurface() const { 
      return image->getSurface();
    }

    virtual void explode();
    ExplodingSprite* getExplosion() const { return explosion; };

    int getScaledWidth()  const { return getScale()*image->getWidth();  } 
    int getScaledHeight() const { return getScale()*image->getHeight(); } 

  private:
    const Image* image;

  protected:
    ExplodingSprite* explosion;

    int worldWidth;
    int worldHeight;

    int getDistance(const Sprite*) const;

};

#endif
