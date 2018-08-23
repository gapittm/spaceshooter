#ifndef BULLET__H
#define BULLET__H

#include <string>
#include "sprite.h"

class Bullet : public Sprite {

  public:
    Bullet(const std::string&);
    virtual ~Bullet() { }

    int getDamage(){ return damage; }
    void setDamage(int v){ damage = v; }

  private:
    int damage;

};

#endif
