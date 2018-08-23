#ifndef PLAYER__H
#define PLAYER__H

#include <vector>
#include <list>
#include "sprite.h"
#include "side.h"
#include "sound.h"

class Enemy;
class Bullet;

class Player : public Sprite {

  /*
      A composite sprite that also acts as the subject for observers.
  */

  public:

    Player(const std::string&);
    Player(const Player&);
    virtual ~Player();

    Player& operator=(const Player&);

    Vector2f getCurVelocity() const { return curVelocity; }
    Vector2f getAcceleration() const { return acceleration; }

    void collided(Side s);

    void attach(Drawable* e) { observers.push_back(e); }
    void detach(Drawable* e);

    Bullet* activate(const std::string&);
    void deactivate(std::list<Bullet*>::iterator);

    std::list<Bullet*>* getActive(){ return &active; }
    std::list<Bullet*>* getInactive(){ return &inactive; }

    void up();
    void down();
    void left();
    void right();
    void stop();
    void shoot();

    int getShield(){ return shield; }
    int getHealth(){ return health; }
    int getScore(){ return score; }
    void setShield(int v);
    void setHealth(int v);
    void setScore(int v);

    void swapWeapon();
    std::string getWeapon(){ return wpnType; }

    void setWpnRotSpd(const float spd){ wpnRotSpd = spd; }
    void wpnClockwise() { setWpnRotSpd(-maxRotSpd); }
    void wpnCounterClockwise(){ setWpnRotSpd(maxRotSpd); }

    bool isShooting() const { return shooting; }
    void setShooting(bool v) { shooting = v; }

    bool isGod() const { return god; }
    void toggleGod() { god = !god; }

    virtual void draw() const;
    virtual void update(Uint32 ticks);

  private:
    Sprite* weapon;
    std::vector<Sprite*> blades;
    std::list<Drawable*> observers;

    std::list<Bullet*> active;
    std::list<Bullet*> inactive;

    int maxVelocity;
    Vector2f curVelocity;
    Vector2f acceleration;
    Vector2f prevPos;

    int shield;
    int health;
    int score;
    std::string wpnType;

    float wpnRotSpd;
    float maxRotSpd;

    float fireRate;
    int ticksPassed;
    bool shooting;

    bool god;
};

#endif
