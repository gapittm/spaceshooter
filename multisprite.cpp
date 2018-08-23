#include "multisprite.h"
#include "gamedata.h"
#include "renderContext.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if(timeSinceLastFrame > frameInterval){
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

MultiSprite::MultiSprite(const std::string& n) :
  Drawable(n, 
           Vector2f(Gamedata::getInstance().getXmlInt(n + "/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(n + "/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(n + "/speedX"),
                    Gamedata::getInstance().getXmlInt(n + "/speedY")),
                    Gamedata::getInstance().getXmlFloat(n + "/angle"),
                    Gamedata::getInstance().getXmlFloat(n + "/speedR")),

  images(RenderContext::getInstance()->getImages(n)),
  currentFrame(0),
  numberOfFrames(Gamedata::getInstance().getXmlInt(n + "/frames")),
  frameInterval(Gamedata::getInstance().getXmlInt(n + "/frameInterval")),
  timeSinceLastFrame(0),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

MultiSprite::MultiSprite(const std::string& n, const Vector2f& pos) :
  Drawable(n, pos, Vector2f(0, 0), 0, 0),
  images(RenderContext::getInstance()->getImages(n)),
  currentFrame(0),
  numberOfFrames(Gamedata::getInstance().getXmlInt(n + "/frames")),
  frameInterval(Gamedata::getInstance().getXmlInt(n + "/frameInterval")),
  timeSinceLastFrame(0),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s), 
  images(s.images),
  currentFrame(s.currentFrame),
  numberOfFrames(s.numberOfFrames),
  frameInterval(s.frameInterval),
  timeSinceLastFrame(s.timeSinceLastFrame),
  worldWidth(s.worldWidth),
  worldHeight(s.worldHeight)
{ }

MultiSprite& MultiSprite::operator=(const MultiSprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  currentFrame = (s.currentFrame);
  numberOfFrames = (s.numberOfFrames);
  frameInterval = (s.frameInterval);
  timeSinceLastFrame = (s.timeSinceLastFrame);
  worldWidth = (s.worldWidth);
  worldHeight = (s.worldHeight);
  return *this;
}

void MultiSprite::draw() const { 
  images[currentFrame]->draw(getX(), getY(), getScale(), getAngle());
}

void MultiSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  setAngle(getAngle() + getRotSpd());

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
  }  

}
