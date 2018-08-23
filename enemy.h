#ifndef ENEMY__H
#define ENEMY__H

#include <string>
#include "sprite.h"
#include "side.h"

class Enemy : public Sprite {

  /*
      A very basic ai that will begin chasing the player once
      it's in range. Once it's locked on, it'll be harder (or
      easier, depending on chaseFactor) for the player to lose
      it.
  */

  public:
    Enemy(const std::string&, const Vector2f&, const int, const int);
    virtual ~Enemy() { }

    Vector2f getTargetPos() const { return targetPos; }
    int getSpeed() const { return speed; }
    float getSight() const { return sightRange; }
    int getHealth() const { return health; }
    int getDamage() const { return damage; }

    void setTargetPos(const Vector2f& p) { targetPos = p; }
    void setSpeed(const int v) { speed = v; }
    void setSight(const float v) { sightRange = v; }
    void setHealth(const int v) { health = v; }
    void setDamage(const int v) { damage = v; }

    virtual void update(Uint32 ticks);

  private:
    enum STATE {IDLE, CHASE};
    STATE currentState;

    Vector2f targetPos;
    int targetWidth;
    int targetHeight;

    Vector2f prevPos;
    int speed;
    float sightRange;
    float chaseFactor;
    int health;
    int damage;

    void up(){ setVelocityY(speed); }
    void down(){ setVelocityY(-speed); }
    void left(){ setVelocityX(-speed); }
    void right(){ setVelocityX(speed); }

};

#endif
