#include <cmath>
#include <functional>
#include "sprite.h"
#include "explodingSprite.h"
#include "gamedata.h"
#include "renderContext.h"

Sprite::Sprite(const std::string &n, const Vector2f &pos, const Vector2f &vel,
               const Image *img, const float a, const float r) :
  Drawable(n, pos, vel, a, r), 
  image(img),
  explosion(NULL),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Sprite::Sprite(const std::string &n, const Vector2f &pos) :
  Drawable(n, pos, Vector2f(0, 0), 0, 0),
  image(RenderContext::getInstance()->getImage(n)),
  explosion(NULL),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Sprite::Sprite(const std::string &n) :
  Drawable(n,
           Vector2f(Gamedata::getInstance().getXmlInt(n + "/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(n + "/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(n + "/speedX"), 
                    Gamedata::getInstance().getXmlInt(n + "/speedY")),
           Gamedata::getInstance().getXmlFloat(n + "/angle"),
           Gamedata::getInstance().getXmlFloat(n + "/speedR")),

  image(RenderContext::getInstance()->getImage(n)),
  explosion(NULL),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Sprite::Sprite(const Sprite &s) :
  Drawable(s), 
  image(s.image),
  explosion(NULL),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Sprite& Sprite::operator=(const Sprite &rhs) {
  Drawable::operator=(rhs);
  image = rhs.image;
  explosion = NULL;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

void Sprite::explode(){
  if(!explosion){
    Sprite sprite(getName(), getPosition(), getVelocity(), image,
      getAngle(), getRotSpd());
    explosion = new ExplodingSprite(sprite);
  }
}

inline namespace {
  constexpr float SCALE_EPSILON = 2e-7;
}

void Sprite::draw() const { 
  if (getScale() < SCALE_EPSILON) return;
  if (explosion) explosion->draw();
  else image->draw(getX(), getY(), getScale(), getAngle()); 
}

void Sprite::update(Uint32 ticks) {
  if(explosion){
    explosion->update(ticks);
    if(explosion->chunkCount() == 0){
      delete explosion;
      explosion = NULL;
    }
  }

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  setAngle(getAngle() + getRotSpd());

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
