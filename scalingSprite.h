#ifndef SCALINGSPRITE__H
#define SCALINGSPRITE__H
#include <string>
#include "drawable.h"
#include "sprite.h"

class ScalingSprite : public Sprite {

  public:

    ScalingSprite(const std::string&);
    ScalingSprite(const std::string&, const Vector2f&, const Vector2f&,
                  const Image*, const float, const float);
    ScalingSprite(const ScalingSprite&);
    virtual ~ScalingSprite() {}
    ScalingSprite& operator=(const ScalingSprite&);

    virtual void update(Uint32 ticks);

  private:

    float scaleSpd;
    float scaleRng;

};

#endif
