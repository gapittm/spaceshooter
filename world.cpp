#include <iostream>
#include "world.h"
#include "renderContext.h"

World::World(const std::string& name, int fact) : 
  image( RenderContext::getInstance()->getImage(name) ),
  factor(fact),
    worldWidth( Gamedata::getInstance().getXmlInt("world/width") ),
    worldHeight( Gamedata::getInstance().getXmlInt("world/height") ),
    imageWidth( image->getWidth() ),
    imageHeight( image->getHeight() ),
    viewX(0.0), viewY(0.0), 
    view(Viewport::getInstance()) 
{ }

void World::update() {
  if(factor > 0){
    viewX = static_cast<int>(view.getX() / factor) % imageWidth;
    viewY = static_cast<int>(view.getY() / factor) % imageHeight;
  }
}

void World::draw() const { 
  image->draw(0,0,static_cast<int>(-viewX),static_cast<int>(-viewY));
  image->draw(0,0,static_cast<int>(imageWidth-viewX),static_cast<int>(-viewY));
  image->draw(0,0,static_cast<int>(-viewX),static_cast<int>(imageHeight-viewY));
  image->draw(0,0,static_cast<int>(imageWidth-viewX),static_cast<int>(imageHeight-viewY));
}

