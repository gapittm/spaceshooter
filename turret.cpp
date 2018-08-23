#include <algorithm>
#include <cstdlib>
#include <cmath>
#include "turret.h"
#include "bullet.h"
#include "gamedata.h"

Turret::Turret(const std::string& n, const Vector2f& turPos, const Vector2f& plrPos,
  const int w, const int h) :
  Sprite(n, turPos),
  targetPos(plrPos),
  targetWidth(w),
  targetHeight(h),
  range(Gamedata::getInstance().getXmlFloat(n + "/range")),
  speed(Gamedata::getInstance().getXmlInt(getName() + "/speed")),
  inaccuracy(Gamedata::getInstance().getXmlInt(getName() + "/inaccuracy")),
  damage(Gamedata::getInstance().getXmlInt(n + "/damage")),
  fireRate(Gamedata::getInstance().getXmlInt(n + "/fireRate")),
  ticksPassed(0),
  ready(false)
{ }

void Turret::shoot(std::list<Bullet*>& bullets){
  if(fireRate > ticksPassed) return;
  ticksPassed = 0;

  Bullet* firedBullet = new Bullet("BasicBullet");

  int wx = getX() + getScaledWidth() / 2;
  int wy = getY() + getScaledHeight() / 2;
  int bx = wx - firedBullet->getScaledWidth() / 2;
  int by = wy - firedBullet->getScaledHeight() / 2;

  int variance = (rand() % inaccuracy) - (inaccuracy / 2);

  float width = getScaledWidth() / 1.5;
  float convAngle = (getAngle() + variance) * (M_PI / 180);

  firedBullet->setDamage(damage);
  firedBullet->setAngle(getAngle() + variance);

  // Some trigonometry to determine what angle the bullets should fly at
  // x = cos(angle) and y = sin(angle)
  firedBullet->setVelocity(Vector2f(cos(convAngle),
    sin((convAngle))).normalize() * speed);

  // Move the bullets forward some so they come out of the end of the
  // weapon barrel instead of the center
  firedBullet->setPosition(Vector2f(bx, by) + Vector2f(cos(convAngle),
    sin((convAngle))).normalize() * width);

  bullets.push_back(firedBullet);
  ready = false;
}

void Turret::update(Uint32 ticks){
  ticksPassed += ticks;

  float angle = atan2((getY() - targetPos[1]), (getX() - targetPos[0]));
  angle *= 180 / M_PI;
  angle += 180;
  setAngle(angle);

  float x = getX() + getImage()->getWidth() / 2;
  float y = getY() + getImage()->getHeight() / 2;
  float tx = targetPos[0] + targetWidth / 2;
  float ty = targetPos[1] + targetHeight / 2;

  float dist = hypot(x - tx, y - ty);
  if(dist < range) ready = true;
}
