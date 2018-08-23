#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <string>
#include "player.h"
#include "enemy.h"
#include "turret.h"
#include "bullet.h"
#include "gamedata.h"

Player::Player(const std::string& n) :
  Sprite(n),
  weapon(new Sprite("PlayerDroneWeapon")),
  blades(),
  observers(),
  active(),
  inactive(),
  maxVelocity(Gamedata::getInstance().getXmlInt(n + "/speed")),
  curVelocity(Vector2f(0, 0)),
  acceleration(Vector2f(Gamedata::getInstance().getXmlInt(n + "/acceleration/x"),
                        Gamedata::getInstance().getXmlInt(n + "/acceleration/y"))),
  prevPos(getPosition()),
  shield(10),
  health(10),
  score(0),
  wpnType("Cannon"),
  wpnRotSpd(0),
  maxRotSpd(Gamedata::getInstance().getXmlFloat(n + "/speedW")),
  fireRate(Gamedata::getInstance().getXmlInt("Cannon/firerate")),
  ticksPassed(0),
  shooting(false),
  god(false)
{
  // Determine how many sets of blades the player drone has
  int bladeNum = Gamedata::getInstance().getXmlInt(n + "/blades");
  if(bladeNum < 1) bladeNum = 1;
  int angleOffset = 90 / bladeNum;
  for(int i = 0; i < bladeNum; i++){
    blades.push_back(new Sprite("PlayerDroneBlade"));
    blades[i]->setAngle(angleOffset * i);
  }

  // Add the minimum number of bullets to the object pool
  int bulletCount = Gamedata::getInstance().getXmlInt(n + "/bullets");
  for(int i = 0; i < bulletCount; i++){
    inactive.push_back(new Bullet("BasicBullet"));
  }
}

Player::Player(const Player& p) :
  Sprite(p),
  weapon(p.weapon),
  blades(p.blades),
  observers(p.observers),
  active(p.active),
  inactive(p.inactive),
  maxVelocity(p.maxVelocity),
  curVelocity(p.curVelocity),
  acceleration(p.acceleration),
  prevPos(p.prevPos),
  shield(p.shield),
  health(p.health),
  score(p.score),
  wpnType(p.wpnType),
  wpnRotSpd(p.wpnRotSpd),
  maxRotSpd(p.maxRotSpd),
  fireRate(p.fireRate),
  ticksPassed(p.ticksPassed),
  shooting(p.shooting),
  god(p.god)
{ }

Player::~Player(){
  delete weapon;
  for(auto it : blades) delete it;
  for(auto it : active) delete it;
  for(auto it : inactive) delete it;
}

Player& Player::operator=(const Player& rhs) {
  Sprite::operator=(rhs);
  weapon = rhs.weapon;
  blades = rhs.blades;
  observers = rhs.observers;
  active = rhs.active;
  inactive = rhs.inactive;
  maxVelocity = rhs.maxVelocity;
  curVelocity = rhs.curVelocity;
  acceleration = rhs.acceleration;
  prevPos = rhs.prevPos;
  shield = rhs.shield;
  health = rhs.health;
  score = rhs.score;
  wpnType = rhs.wpnType;
  wpnRotSpd = rhs.wpnRotSpd;
  maxRotSpd = rhs.maxRotSpd;
  fireRate = rhs.fireRate;
  ticksPassed = rhs.ticksPassed;
  shooting = rhs.shooting;
  god = rhs.god;
  return *this;
}

void Player::collided(Side s){
  Vector2f curPos = getPosition();

  switch(s){
    case Side::TOP:
      setPosition(Vector2f(curPos[0], curPos[1]+15));
      break;

    case Side::BOTTOM:
      setPosition(Vector2f(curPos[0], curPos[1]-15));
      break;

    case Side::LEFT:
      setPosition(Vector2f(curPos[0]+15, curPos[1]));
      break;

    case Side::RIGHT:
      setPosition(Vector2f(curPos[0]-15, curPos[1]));
      break;

    default:
      break;
  }

  for(auto it : blades) it->setPosition(getPosition());
  weapon->setPosition(getPosition());
}

void Player::detach(Drawable* e) {
  auto it = observers.begin();
  while(it != observers.end()){
    if(*it == e){
      it = observers.erase(it);
      return;
    }
    
    it++;
  }
}

// Pull a bullet out of the object pool for use
Bullet* Player::activate(const std::string& n) {
  std::list<Bullet*>::iterator it = std::find_if(inactive.begin(), inactive.end(), 
    [&n](Bullet* d)->bool{ return d->getName() == n; });

  if(it == inactive.end()){
    Bullet* newActive = new Bullet(n);
    active.push_back(newActive);
    return newActive;
  } else {
    active.splice(active.end(), inactive, it);
    return *it;
  }
}

// Place an active bullet back into the pool
void Player::deactivate(std::list<Bullet*>::iterator it) {
  inactive.splice(inactive.begin(), active, it);
}

void Player::up() {
  if(getY() > 0 && curVelocity[1] > -maxVelocity) {
    curVelocity[1] -= acceleration[1];
    setVelocityY(curVelocity[1]);
  }
}

void Player::down() {
  if(getY() < worldHeight - getScaledHeight() &&
    curVelocity[1] < maxVelocity)
  {
    curVelocity[1] += acceleration[1];
    setVelocityY(curVelocity[1]);
  }
}

void Player::left() {
  if(getX() > 0 && curVelocity[0] > -maxVelocity){
    curVelocity[0] -= acceleration[0];
    setVelocityX(curVelocity[0]);
  }
}

void Player::right() {
  if(getX() < worldWidth - getScaledWidth() &&
    curVelocity[0] < maxVelocity)
  {
    curVelocity[0] += acceleration[0];
    setVelocityX(curVelocity[0]);
  }
}

void Player::stop() {
  if(getX() > worldWidth - getScaledWidth() || getX() < 0) curVelocity[0] = 0;
  if(getY() > worldHeight - getScaledHeight() || getY() < 0) curVelocity[1] = 0;
  if(curVelocity[0] > 0) curVelocity[0] -= acceleration[0] / 2;
  if(curVelocity[0] < 0) curVelocity[0] += acceleration[0] / 2;
  if(curVelocity[1] > 0) curVelocity[1] -= acceleration[1] / 2;
  if(curVelocity[1] < 0) curVelocity[1] += acceleration[1] / 2;
  setVelocity(curVelocity);
}

void Player::shoot() {
  if(fireRate > ticksPassed) return;
  ticksPassed = 0;
  shooting = true;

  int count = Gamedata::getInstance().getXmlInt(wpnType + "/count");

  for(int i = 0; i < count; i++){
    Bullet* firedBullet = activate("BasicBullet");

    int wx = weapon->getX() + weapon->getScaledWidth() / 2;
    int wy = weapon->getY() + weapon->getScaledHeight() / 2;
    int bx = wx - firedBullet->getScaledWidth() / 2;
    int by = wy - firedBullet->getScaledHeight() / 2;

    int inaccuracy = Gamedata::getInstance().getXmlFloat(wpnType + "/inaccuracy");
    int variance = (rand() % inaccuracy) - (inaccuracy / 2);

    float speed = Gamedata::getInstance().getXmlFloat(wpnType + "/speed");
    float width = weapon->getScaledWidth() / 1.5;
    float convAngle = (weapon->getAngle() + variance) * (M_PI / 180);

    firedBullet->setDamage(Gamedata::getInstance().getXmlInt(wpnType + "/damage"));
    firedBullet->setAngle(weapon->getAngle() + variance);

    // Some trigonometry to determine what angle the bullets should fly at
    // x = cos(angle) and y = sin(angle)
    firedBullet->setVelocity(Vector2f(cos(convAngle),
      sin((convAngle))).normalize() * speed);

    // Move the bullets forward some so they come out of the end of the
    // weapon barrel instead of the center
    firedBullet->setPosition(Vector2f(bx, by) + Vector2f(cos(convAngle),
      sin((convAngle))).normalize() * width);
  }
}

void Player::setShield(int val){
  shield = val;
  if(shield > 10) shield = 10;
  if(shield < 0) shield = 0;
}

void Player::setHealth(int val){
  health = val;
  if(health > 10) health = 10;
  if(health < 0) health = 0;
}

void Player::setScore(int val){
  score = val;
  if(score > 20) score = 20;
  if(score < 0) score = 0;
}

void Player::swapWeapon(){
  if(wpnType == "Cannon")
    wpnType = "Shotgun";

  else if(wpnType == "Shotgun")
    wpnType = "Sniper";

  else if(wpnType == "Sniper")
    wpnType = "Cannon";

  fireRate = Gamedata::getInstance().getXmlInt(wpnType + "/firerate");
}

void Player::draw() const {
  for(auto it : blades) it->draw();
  Sprite::draw();
  for(auto it : active) it->draw();
  weapon->draw();
}

void Player::update(Uint32 ticks) {
  ticksPassed += ticks;

  float mult = maxVelocity / getVelocity().magnitude();
  if(mult < 3)
    setVelocity(getVelocity() * mult);

  prevPos = getPosition();
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  setAngle(getAngle() + getRotSpd());

  for(auto it : blades){
    it->update(ticks);
    it->setPosition(getPosition());
  }

  weapon->update(ticks);
  weapon->setPosition(getPosition());
  weapon->setAngle(weapon->getAngle() + wpnRotSpd);
  setWpnRotSpd(0);

  for(auto it : active) it->update(ticks);
  for(auto it : observers){
    Enemy* test = dynamic_cast<Enemy*>(it);

    if(test)
      static_cast<Enemy*>(it)->setTargetPos(getPosition());

    else static_cast<Turret*>(it)->setTargetPos(getPosition());
  }

  stop();
}
