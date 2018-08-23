#include "scalingSprite.h"
#include "gamedata.h"
#include "renderContext.h"

ScalingSprite::ScalingSprite(const string& n, const Vector2f& pos,
                             const Vector2f& vel, const Image* img,
                             const float a, const float r) :
  Sprite(n, pos, vel, img, a, r),
  scaleSpd(Gamedata::getInstance().getXmlFloat(n + "/speedS")),
  scaleRng(Gamedata::getInstance().getXmlFloat(n + "/scaleRange"))
{ }

ScalingSprite::ScalingSprite(const string& n) :
  Sprite(n),
  scaleSpd(Gamedata::getInstance().getXmlFloat(n + "/speedS")),
  scaleRng(Gamedata::getInstance().getXmlFloat(n + "/scaleRange"))
{ }

ScalingSprite::ScalingSprite(const ScalingSprite& s) :
  Sprite(s),
  scaleSpd(s.scaleSpd),
  scaleRng(s.scaleRng)
{ }

ScalingSprite& ScalingSprite::operator=(const ScalingSprite& rhs) {
  Sprite::operator=(rhs);
  scaleSpd = rhs.scaleSpd;
  scaleRng = rhs.scaleRng;
  return *this;
}

void ScalingSprite::update(Uint32 ticks) {
  Sprite::update(ticks);
  setScale(getScale() + scaleSpd);

  if(getScale() < 1.0 - scaleRng || getScale() > 1.0 + scaleRng)
    scaleSpd *= -1;
}
