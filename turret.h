#ifndef TURRET__H
#define TURRET__H

#include <string>
#include <list>
#include "sprite.h"

class Bullet;

class Turret : public Sprite {

  public:
    Turret(const std::string&, const Vector2f&, const Vector2f&, 
      const int, const int);
    virtual ~Turret() { }

    void setTargetPos(const Vector2f& p) { targetPos = p; }

    bool isReady() const { return ready; }

    void shoot(std::list<Bullet*>&);

    virtual void update(Uint32 ticks);

  private:
    Vector2f targetPos;
    int targetWidth;
    int targetHeight;

    float range;
    int speed;
    int inaccuracy;
    int damage;
    int fireRate;
    int ticksPassed;
    bool ready;
};

#endif
