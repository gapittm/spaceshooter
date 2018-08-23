#include <SDL_image.h>
#include "ioMod.h"
#include "gamedata.h"
#include "renderContext.h"

IOmod& IOmod::getInstance() {
  static IOmod instance;
  return instance;
}

IOmod::~IOmod() {
  for(auto f : fonts) TTF_CloseFont(f.second.first);
  TTF_Quit(); 
}

IOmod::IOmod() : 
  init(TTF_Init()),
  renderer(RenderContext::getInstance()->getRenderer()),
  fonts()
{
  if ( init == -1 )
    throw std::string("error: Couldn't init font");

  setFont("defaultfont");
  setFont("weaponfont");
}

SDL_Texture* IOmod::readTexture(const std::string& filename) {
  SDL_Texture *texture = IMG_LoadTexture(renderer, filename.c_str());

  if ( texture == NULL ) {
    throw std::string("Couldn't load ") + filename;
  }

  return texture;
}

SDL_Surface* IOmod::readSurface(const std::string& filename) {
  SDL_Surface *surface = IMG_Load(filename.c_str());

  if ( !surface ) {
    throw std::string("Couldn't load ") + filename;
  }

  return surface;
}

void IOmod::writeText(const std::string& msg, int x, int y, const std::string& font) const {
  SDL_Surface* surface = 
    TTF_RenderText_Solid(IOmod::getFont(font)->second.first,
    msg.c_str(), IOmod::getFont(font)->second.second);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = surface->w;
  int textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {x, y, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}

void IOmod::setFont(const std::string& name) {
  SDL_Color fontColor = {0,0,0,0};
  fontColor.r = Gamedata::getInstance().getXmlInt(name + "/red");
  fontColor.g = Gamedata::getInstance().getXmlInt(name + "/green");
  fontColor.b = Gamedata::getInstance().getXmlInt(name + "/blue");
  fontColor.a = Gamedata::getInstance().getXmlInt(name + "/alpha");

  fonts.push_back(std::make_pair(name,
    std::make_pair(TTF_OpenFont(Gamedata::getInstance().getXmlStr(name + "/file").c_str(),
      Gamedata::getInstance().getXmlInt(name + "/size")),
    fontColor)));
}

std::vector<std::pair<std::string, std::pair<TTF_Font*, SDL_Color>>>::const_iterator
  IOmod::getFont(const std::string& name) const
{
  std::vector<std::pair<std::string, std::pair<TTF_Font*, SDL_Color>>>::const_iterator it;
  for(it = fonts.begin(); it != fonts.end(); it++)
    if(it->first == name) return it;
  
  throw std::string("error: Font " + name + " not found");
}
