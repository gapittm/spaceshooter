#include <cmath>
#include <vector>
#include <algorithm>
#include "collision.h"
#include "viewport.h"
#include "renderContext.h"

Side Rectangular::execute(const Drawable& s1, const Drawable& s2) const {
  float left1 = s1.getX();
  float left2 = s2.getX();
  float right1 = left1 + s1.getScaledWidth();
  float right2 = left2 + s2.getScaledWidth();

  if(right1 < left2) return Side::NONE;
  if(left1 > right2) return Side::NONE;

  float top1 = s1.getY();
  float top2 = s2.getY();
  float bottom1 = top1 + s1.getScaledHeight();
  float bottom2 = top2 + s2.getScaledHeight();

  if(bottom1 < top2) return Side::NONE;
  if(bottom2 < top1) return Side::NONE;

  return Side::EXISTS;
}

Side Directional::execute(const Drawable& s1, const Drawable& s2) const {
  float avgW = (s1.getScaledWidth() + s2.getScaledWidth()) / 2;
  float avgH = (s1.getScaledHeight() + s2.getScaledHeight()) / 2;
  float diffX = s2.getX() - s1.getX();
  float diffY = s2.getY() - s1.getY();

  if(std::abs(diffX) <= avgW && std::abs(diffY) <= avgH){
    float wy = avgW * diffY;
    float hx = avgH * diffX;

    if(wy > hx){

      if(wy > -hx){
        return Side::BOTTOM;
      } else {
        return Side::LEFT;
      }

    } else {

      if(wy > -hx){
        return Side::RIGHT;
      } else {
        return Side::TOP;
      }

    }
  }

  return Side::NONE;
}

SDL_Surface* scaleSurface(const SDL_Surface* const surf, int width, int height) {

  const auto* fmt = surf->format;
  SDL_Surface* sub = SDL_CreateRGBSurface(0,width,height,
                     fmt->BitsPerPixel,fmt->Rmask,fmt->Gmask,
                     fmt->Bmask,fmt->Amask);

  // Get the old mode
  SDL_BlendMode oldBlendMode;
  SDL_GetSurfaceBlendMode(const_cast<SDL_Surface*>(surf), &oldBlendMode);

  // Set the new mode so copying the source won't change the source
  SDL_SetSurfaceBlendMode(const_cast<SDL_Surface*>(surf), SDL_BLENDMODE_NONE);

  SDL_Rect rect = {0,0,width,height};
  int flag = SDL_BlitScaled(const_cast<SDL_Surface*>(surf),nullptr,sub,&rect);
  if ( flag < 0 ) throw( std::string("SDL_BlitScaled failed!") );

  // Set the blend mode back to original
  SDL_SetSurfaceBlendMode(const_cast<SDL_Surface*>(surf), oldBlendMode);
  return sub;
}

bool PerPixel::isVisible(Uint32 pixel, SDL_Surface *surface) const {
	Uint32 temp;
	SDL_PixelFormat* fmt = surface->format;

	if(fmt->BitsPerPixel == 32){
		temp=pixel&fmt->Amask;  // Isolate alpha component
		temp=temp>>fmt->Ashift; // Shift it down to 8-bit
		temp=temp<<fmt->Aloss;  // Expand to a full 8-bit number

		if(temp == 0) return false;
	}

	return true;
}

Side PerPixel::execute(const Drawable& s1, const Drawable& s2) const {

  Directional strategy;
  Side test = strategy.execute(s1, s2);
  if(test == Side::NONE) return Side::NONE;

  Vector2f p1 = s1.getPosition() - Viewport::getInstance().getPosition();
  Vector2f p2 = s2.getPosition() - Viewport::getInstance().getPosition();

  Uint16 width1 = s1.getScaledWidth();
  Uint16 height1 = s1.getScaledHeight();

  Uint16 width2 = s2.getScaledWidth();
  Uint16 height2 = s2.getScaledHeight();

  int o1Left = p1[0]; 
  int o1Right = o1Left+width1;

  int o2Left = p2[0]; 
  int o2Right = o2Left+width2;
  std::vector<int> sides;
  sides.reserve(4);
  sides.push_back( o1Left );
  sides.push_back( o1Right );
  sides.push_back( o2Left );
  sides.push_back( o2Right );
  std::sort( sides.begin(), sides.end() );

  int o1Up = p1[1];
  int o1Down = o1Up+height1;
  int o2Up = p2[1];
  int o2Down = o2Up+height2;
  std::vector<int> lids;
  lids.reserve(4);
  lids.push_back( o1Up );
  lids.push_back( o1Down );
  lids.push_back( o2Up );
  lids.push_back( o2Down );
  std::sort( lids.begin(), lids.end() );

  const SDL_Surface* temp1 = s1.getSurface(); 
  SDL_Surface* surface1 = scaleSurface(temp1, width1, height1);
  const SDL_Surface* temp2 = s2.getSurface(); 
  SDL_Surface* surface2 = scaleSurface(temp2, width2, height2);

  SDL_LockSurface(surface1);
  SDL_LockSurface(surface2);
  Uint32 *pixels1 = static_cast<Uint32 *>(surface1->pixels);
  Uint32 *pixels2 = static_cast<Uint32 *>(surface2->pixels);
  unsigned pixel1;
  unsigned pixel2;
  for(int x = sides[1]; x < sides[2]; ++x){
    for(int y = lids[1]; y < lids[2]; ++y){
      // Check pixels in surface for s1 and surface for s2
      int i = x - p1[0];
      int j = y - p1[1];
      pixel1 = pixels1[ (j * width1) + i ];
      i = x - p2[0];
      j = y - p2[1];
      pixel2 = pixels2[ (j * width2) + i ];

      if(isVisible(pixel1,surface1) && isVisible(pixel2,surface2)){
        SDL_UnlockSurface(surface1);
        SDL_UnlockSurface(surface2);
        SDL_FreeSurface(surface1);
        SDL_FreeSurface(surface2);
        return test;
      }
    }
  }

  SDL_UnlockSurface(surface1);
  SDL_UnlockSurface(surface2);
  SDL_FreeSurface(surface1);
  SDL_FreeSurface(surface2);

  return Side::NONE;
}
