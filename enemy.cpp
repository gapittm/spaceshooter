#include <cmath>
#include <random>
#include <functional>
#include <iostream>
#include "enemy.h"
#include "gamedata.h"
#include "renderContext.h"

Enemy::Enemy(const std::string& n, const Vector2f& p, const int w, const int h) :
  Sprite(n),
  currentState(IDLE),
  targetPos(p),
  targetWidth(w),
  targetHeight(h),
  prevPos(getPosition()),
  speed(Gamedata::getInstance().getXmlInt(n + "/speed")),
  sightRange(Gamedata::getInstance().getXmlFloat(n + "/sightRange")),
  chaseFactor(Gamedata::getInstance().getXmlFloat(n + "/chaseFactor")),
  health(Gamedata::getInstance().getXmlInt(n + "/health")),
  damage(Gamedata::getInstance().getXmlInt(n + "/damage"))
{ }

void Enemy::update(Uint32 ticks) {
  float mult = speed / getVelocity().magnitude();
  if(mult < 3)
    setVelocity(getVelocity() * mult);

  prevPos = getPosition();
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  setAngle(getAngle() + getRotSpd());

  float x = getX() + getImage()->getWidth() / 2;
  float y = getY() + getImage()->getHeight() / 2;
  float tx = targetPos[0] + targetWidth / 2;
  float ty = targetPos[1] + targetHeight / 2;

  float dist = hypot(x - tx, y - ty);

  if(currentState == IDLE && dist < sightRange){
    currentState = CHASE;
    setRotSpd(getRotSpd() * 3);
  } else if(currentState == CHASE) {
    if(dist > sightRange * chaseFactor) {
      currentState = IDLE;
      setVelocity(Vector2f(0, 0));
      setRotSpd(getRotSpd() / 3);
    } else {
      if(x > tx && abs(x - tx) > 64) left();
      else if(x < tx && x + tx > 64) right();
      if(y < ty && abs(y - ty) > 64) up();
      else if(y > ty && y + ty > 64) down();
    }
  }

  if(getY() < 0){
    setVelocityY(std::abs(getVelocityY()));
  }
  if(getY() > worldHeight-getScaledHeight()){
    setVelocityY(-std::abs( getVelocityY()));
  }

  if(getX() < 0){
    setVelocityX(std::abs(getVelocityX()));
  }
  if(getX() > worldWidth-getScaledWidth()){
    setVelocityX(-std::abs(getVelocityX()));
  }  
}
