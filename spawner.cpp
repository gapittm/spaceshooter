#include <algorithm>
#include <cstdlib>
#include "spawner.h"
#include "player.h"
#include "enemy.h"
#include "gamedata.h"

int Spawner::curEnemies = 0;

Spawner::Spawner(const std::string &n, const Vector2f& pos, Player* plr,
  std::vector<Enemy*>* e) : 
  MultiSprite(n, pos),
  ticksPassed(0),
  spawnInterval(Gamedata::getInstance().getXmlInt(n + "/spawnInterval")),
  spawnFactor(Gamedata::getInstance().getXmlFloat(n + "/spawnFactor")),
  typeChance(Gamedata::getInstance().getXmlInt(n + "/typeChance")),
  maxEnemies(Gamedata::getInstance().getXmlInt(n + "/maxEnemies")),
  enemies(e),
  target(plr)
{ }

void Spawner::spawn(){
  if(spawnInterval > ticksPassed || curEnemies >= maxEnemies) return;

  Enemy* newSpawn;

  // Modify this to include an additional mobile enemy type
  if(rand() % typeChance == 1){
    newSpawn = new Enemy("Bomber", target->getPosition(),
      target->getScaledWidth(), target->getScaledHeight());
  } else {
    newSpawn = new Enemy("Bomber", target->getPosition(),
      target->getScaledWidth(), target->getScaledHeight());
  }

  Vector2f spawnerCenter = getPosition() + 
    Vector2f(getScaledWidth() / 2, getScaledHeight() / 2);
  Vector2f newSpawnOffset =
    Vector2f(newSpawn->getScaledWidth() / 2, newSpawn->getScaledHeight() / 2);

  newSpawn->setPosition(spawnerCenter - newSpawnOffset);
  newSpawn->setSpeed(newSpawn->getSpeed() * spawnFactor);
  newSpawn->setSight(newSpawn->getSight() * spawnFactor);

  target->attach(newSpawn);
  enemies->push_back(newSpawn);

  ticksPassed = 0;
  curEnemies++;
}

void Spawner::update(Uint32 ticks){
  MultiSprite::update(ticks);
  ticksPassed += ticks; 
  spawn();
}
