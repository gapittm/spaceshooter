#include "drawable.h"
#include "side.h"

class Collision {

  public:
    virtual Side execute(const Drawable&, const Drawable&) const = 0;
    virtual ~Collision() { }

};

class Rectangular : public Collision {

  public:
    Rectangular() { }
    virtual Side execute(const Drawable&, const Drawable&) const;

};

class Directional : public Collision {

  public:
    Directional() { }
    virtual Side execute(const Drawable&, const Drawable&) const;

};

class PerPixel : public Collision {

  public:
    PerPixel() { }
    virtual Side execute(const Drawable&, const Drawable&) const;

  private:
    bool isVisible(Uint32, SDL_Surface*) const;

};
