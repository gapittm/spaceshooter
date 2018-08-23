#ifndef SPAWNER__H
#define SPAWNER__H

#include <string>
#include <vector>
#include "multisprite.h"

class Player;
class Enemy;

class Spawner : public MultiSprite {

  public:
    Spawner(const std::string&, const Vector2f&, Player*,
      std::vector<Enemy*>*);
    virtual ~Spawner() { }

    static int curEnemies;

    void spawn();

    virtual void update(Uint32 ticks);

  private:

    int ticksPassed;
    int spawnInterval;
    float spawnFactor;
    int typeChance;
    int maxEnemies;

    std::vector<Enemy*>* enemies;
    Player* target;

    Spawner(const Spawner&) = delete;
    Spawner& operator=(const Spawner&) = delete;
};

#endif
