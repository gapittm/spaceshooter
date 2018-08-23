#include "hud.h"
#include "ioMod.h"
#include "gamedata.h"
#include "viewport.h"

const int MAXBARS = 10;

Hud::Hud(const std::string& n) :
  Sprite(n, Vector2f(0, 0)),
  fields(),
  essentials(),
  shield(),
  health(),
  score(),
  plrShield(0),
  plrHealth(0),
  plrScore(0),
  plrWeapon("Cannon"),
  padding(Gamedata::getInstance().getXmlInt(n + "/padding")),
  enabled(true)
{
  for(int i = 0; i < MAXBARS; i++){
    shield.push_back(new Sprite(n + "/shield", Vector2f(0, 0)));
    health.push_back(new Sprite(n + "/health", Vector2f(0, 0)));
    score.push_back(new Sprite(n + "/score", Vector2f(0, 0)));
  }

  fields["Pause"] = "P";
  fields["Move"] = "WASD";
  fields["Rotate"] = "LEFT/RIGHT";
  fields["Fire"] = "SPACE";
  fields["Swap"] = "SHIFT";
  fields["Record"] = "F4";
  fields["God"] = "G";
  essentials["Controls"] = "F1";
  essentials["Quit"] = "ESC";
}

Hud::~Hud(){
  for(auto it : shield) delete it;
  for(auto it : health) delete it;
  for(auto it : score) delete it;
}

void Hud::setShield(int val){
  if(val > 10) val = 10;
  if(val < 0) val = 0;
  plrShield = val;
}

void Hud::setHealth(int val){
  if(val > 10) val = 10;
  if(val < 0) val = 0;
  plrHealth = val;
}

void Hud::setScore(int val){
  if(val > 10) val = 10;
  if(val < 0) val = 0;
  plrScore = val;
}

void Hud::draw(){
  setPosition(Viewport::getInstance().getPosition());
  Sprite::draw();

  for(int i = 0; i < plrShield; i++){
    shield[i]->setPosition(getPosition() + Vector2f(43 + (40 * i), 8));
    shield[i]->draw();
  }

  for(int i = 0; i < plrHealth; i++){
    health[i]->setPosition(getPosition() + Vector2f(43 + (40 * i), 40));
    health[i]->draw();
  }

  for(int i = 0; i < plrScore; i++){
    score[i]->setPosition(getPosition() + Vector2f(43 + (40 * i), 72));
    score[i]->draw();
  }

  IOmod::getInstance().writeText(plrWeapon, 45, 102, "weaponfont");

  IOmod::getInstance().writeText("Quit: ", 1000, 16, "defaultfont");
  IOmod::getInstance().writeText(essentials["Quit"], 1100, 16, "defaultfont");
  IOmod::getInstance().writeText("Controls: ", 1000, 40, "defaultfont");
  IOmod::getInstance().writeText(essentials["Controls"], 1100, 40, "defaultfont");

  if(enabled){
    IOmod::getInstance().writeText("Pause: ", 1000, 64, "defaultfont");
    IOmod::getInstance().writeText(fields["Pause"], 1100, 64, "defaultfont");
    IOmod::getInstance().writeText("Move: ", 1000, 88, "defaultfont");
    IOmod::getInstance().writeText(fields["Move"], 1100, 88, "defaultfont");
    IOmod::getInstance().writeText("Rotate: ", 1000, 112, "defaultfont");
    IOmod::getInstance().writeText(fields["Rotate"], 1100, 112, "defaultfont");
    IOmod::getInstance().writeText("Fire: ", 1000, 136, "defaultfont");
    IOmod::getInstance().writeText(fields["Fire"], 1100, 136, "defaultfont");
    IOmod::getInstance().writeText("Swap: ", 1000, 160, "defaultfont");
    IOmod::getInstance().writeText(fields["Swap"], 1100, 160, "defaultfont");
    IOmod::getInstance().writeText("Record: ", 1000, 184, "defaultfont");
    IOmod::getInstance().writeText(fields["Record"], 1100, 184, "defaultfont");
    IOmod::getInstance().writeText("God: ", 1000, 208, "defaultfont");
    IOmod::getInstance().writeText(fields["God"], 1100, 208, "defaultfont");
  }
}
