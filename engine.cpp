#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "scalingSprite.h"
#include "explodingSprite.h"
#include "hud.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "turret.h"
#include "bullet.h"
#include "spawner.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"

Engine::~Engine() {
  delete hud;
  delete player;
  delete sound;
  for(auto it : explosions) delete it;
  for(auto it : strategy) delete it;
  for(auto it : enemies) delete it;
  for(auto it : tiles) delete it;
  for(auto it : turrets) delete it;
  for(auto it : turretBullets) delete it;
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc(RenderContext::getInstance()),
  io(IOmod::getInstance()),
  hud(new Hud("hud")),
  clock(Clock::getInstance()),
  renderer(rc->getRenderer()),
  sound(new SDLSound()),
  level(1),
  world("back", Gamedata::getInstance().getXmlInt("back/factor")),
  viewport(Viewport::getInstance()),
  player(new Player("PlayerDroneFrame")),
  strategy(),
  tiles(),
  enemies(),
  turrets(),
  turretBullets(),
  explosions(),
  makeVideo( false )
{
  strategy.push_back(new Rectangular());
  strategy.push_back(new Directional());
  strategy.push_back(new PerPixel());

  addTiles(level);
  initPlayer();

  hud->setShield(player->getShield());
  hud->setHealth(player->getHealth());
  hud->setScore(player->getScore());
  hud->setWeapon(player->getWeapon());

  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::addTiles(const int n) {
  char c;
  unsigned int x = 0, y = 0;
  const unsigned int size = Gamedata::getInstance().
                              getXmlInt("tiles/level" + std::to_string(n) + "/size");
  
  const std::string file = Gamedata::getInstance().
                              getXmlStr("tiles/level" + std::to_string(n) + "/file");
  std::ifstream in;
  in.open(file);

  Turret* newTurret;

  while(in.get(c)) {
    switch(c){
      case 'w':
        tiles.push_back(new Sprite("Wall", Vector2f(x * 128, y * 128)));
        break;

      case 'b':
        tiles.push_back(new Sprite("Basic", Vector2f(x * 128, y * 128)));
        break;

      case 's':
        tiles.push_back(new Sprite("Single", Vector2f(x * 128, y * 128)));
        break;

      case '1':
        tiles.push_back(new Sprite("WindowTopLeft", Vector2f(x * 128, y * 128)));
        break;

      case '2':
        tiles.push_back(new Sprite("WindowTopRight", Vector2f(x * 128, y * 128)));
        break;

      case '3':
        tiles.push_back(new Sprite("WindowBottomLeft", Vector2f(x * 128, y * 128)));
        break;

      case '4':
        tiles.push_back(new Sprite("WindowBottomRight", Vector2f(x * 128, y * 128)));
        break;

      case 'p':
        tiles.push_back(new MultiSprite("PlayerSpawner", Vector2f(x * 128, y * 128)));
        break;

      case 'e':
        tiles.push_back(new Spawner("EnemySpawner", Vector2f(x * 128, y * 128),
          player, &enemies));
        break;

      case 't':
        tiles.push_back(new Sprite("TurretBase", Vector2f(x * 128, y * 128)));
        newTurret = new Turret("Turret", Vector2f(x * 128, y * 128),
          player->getPosition(), player->getScaledWidth(), player->getScaledHeight());
        turrets.push_back(newTurret);
        player->attach(newTurret);
        break;

      default:
        break;
    }

    if(x == size){
      x = 0;
      y++;
    }

    else x++;

  }

  in.close();
}

void Engine::initPlayer() {
  for(auto it : tiles)
    if(it->getName() == "PlayerSpawner")
      player->setPosition(it->getPosition());

  player->setShield(10);
  player->setScore(0);
}

void Engine::draw() const {
  world.draw();
  for(auto it : tiles) it->draw();
  for(auto it : turrets) it->draw();
  for(auto it : turretBullets) it->draw();
  for(auto it : enemies) it->draw();

  // Make sure not to draw the explosions once they're
  // done exploding (they would briefly flash before
  // being deleted)
  for(auto it : explosions){
    if(((Sprite*)it)->getExplosion() != NULL) it->draw();
  }

  player->draw();
  hud->draw();

  if(player->getScore() == 10)
    IOmod::getInstance().writeText("Win!", 640, 680, "defaultfont");

  if(player->getHealth() == 0)
    IOmod::getInstance().writeText("Lose!", 640, 680, "defaultfont");

  viewport.draw();
  SDL_RenderPresent(renderer);

  if(player->getScore() == 10 || player->getHealth() == 0){
    clock.pause();
    return;
  }
}

void Engine::update(Uint32 ticks) {
  // Get rid of explosions that are done exploding
  auto it = explosions.begin();
  while(it != explosions.end()){
    if(((Sprite*)*it)->getExplosion() == NULL){
      it = explosions.erase(it);
    } else {
      (*it)->update(ticks);
      it++;
    }
  }

  world.update();

  if(player->isShooting()){
    player->setShooting(false);
    if(player->getWeapon() == "Cannon") (*sound)[0];
    if(player->getWeapon() == "Shotgun") (*sound)[2];
    if(player->getWeapon() == "Sniper") (*sound)[3];
  }
  player->update(ticks);

  for(auto it : turrets){
    if(it->isReady()){
      it->shoot(turretBullets);
    }

    it->update(ticks);
  }
  for(auto it : turretBullets) it->update(ticks);

  for(auto it : enemies) it->update(ticks);
  for(auto it : tiles) it->update(ticks);

  collisionCheck();

  hud->setShield(player->getShield());
  hud->setHealth(player->getHealth());
  hud->setScore(player->getScore());
  hud->setWeapon(player->getWeapon());
  hud->update(ticks);
  viewport.update();
}

void Engine::reset(){
  level++;
  for(auto it : explosions) delete it;
  for(auto it : enemies) delete it;
  for(auto it : turrets) delete it;
  for(auto it : turretBullets) delete it;
  for(auto it : tiles) delete it;
  addTiles(level);
  initPlayer();
}

void Engine::collisionCheck(){
  Side colSide;

  // Check for player collisions with enemies (Rectangular)
  std::vector<Enemy*>::iterator enemy = enemies.begin();
  if(player->isGod() == false){
    while(enemy != enemies.end()){
      if(strategy[0]->execute(*player, **enemy) != Side::NONE){
        Sprite* sprite = new Sprite((*enemy)->getName(),
                (*enemy)->getPosition(), (*enemy)->getVelocity(),
                (*enemy)->getImage(), (*enemy)->getAngle(),
                (*enemy)->getRotSpd());

        sprite->explode();
        explosions.push_back(sprite);

        player->detach(*enemy);
        delete *enemy;
        enemy = enemies.erase(enemy);

        if(player->getShield() == 0){
          player->setHealth(player->getHealth() - (*enemy)->getDamage());
        } else {
          player->setShield(player->getShield() - (*enemy)->getDamage());
        }

        (*sound)[0];
        Spawner::curEnemies--;
      } else enemy++;
    }
  }
  // Check for player collisions with walls (Per Pixel)
  for(auto wall : tiles){
    if(wall->getName() == "Wall"){
      colSide = strategy[2]->execute(*player, *wall);
      if(colSide != Side::NONE){
        player->collided(colSide);
      }
    }
  }

  // Check for player collisions with bullets (Rectangular)
  std::list<Bullet*>::iterator bullet = turretBullets.begin();
  if(player->isGod() == false){
    while(bullet != turretBullets.end()){
      if(strategy[0]->execute(**bullet, *player) != Side::NONE){
        Sprite *sprite = new Sprite((*bullet)->getName(),
                (*bullet)->getPosition(), (*bullet)->getVelocity(),
                (*bullet)->getImage(), (*bullet)->getAngle(),
                (*bullet)->getRotSpd());
              
        sprite->explode();
        explosions.push_back(sprite);

        if(player->getShield() == 0){
          player->setHealth(player->getHealth() - (*bullet)->getDamage());
        } else {
          player->setShield(player->getShield() - (*bullet)->getDamage());
        }

        delete *bullet;
        bullet = turretBullets.erase(bullet);

      } else bullet++;
    }
  }

  // Check for bullet collisions with walls (Rectangular)
  std::list<Bullet*>* bulletList = player->getActive();
  for(auto tile : tiles){
    bullet = bulletList->begin();

    if(tile->getName() == "Wall"){
      while(bullet != bulletList->end()){
        if(strategy[0]->execute(**bullet, *tile) != Side::NONE){
            Sprite *sprite = new Sprite((*bullet)->getName(),
              (*bullet)->getPosition(), (*bullet)->getVelocity(),
              (*bullet)->getImage(), (*bullet)->getAngle(),
              (*bullet)->getRotSpd());
            
            sprite->explode();
            explosions.push_back(sprite);

            player->deactivate(bullet);
            break;
        }

        bullet++;
      }
    }
  }

  // Check for turret bullet collisions with walls (Rectangular)
  for(auto tile : tiles){
    bullet = turretBullets.begin();

    if(tile->getName() == "Wall"){
      while(bullet != turretBullets.end()){
        if(strategy[0]->execute(**bullet, *tile) != Side::NONE){
            Sprite *sprite = new Sprite((*bullet)->getName(),
              (*bullet)->getPosition(), (*bullet)->getVelocity(),
              (*bullet)->getImage(), (*bullet)->getAngle(),
              (*bullet)->getRotSpd());
            
            sprite->explode();
            explosions.push_back(sprite);

            delete *bullet;
            bullet = turretBullets.erase(bullet);
        } else bullet++;
      }
    }
  }

  // Check for bullet collisions with enemies (Rectangular)
  bullet = bulletList->begin();
  enemy = enemies.begin();
  bool delEnemy = false;
  while(enemy != enemies.end()){
    if((*enemy)->getName() == "Bomber"){
      bullet = bulletList->begin();

      while(bullet != bulletList->end()){
        if(strategy[0]->execute(**bullet, **enemy) != Side::NONE){
            Sprite *sprite = new Sprite((*bullet)->getName(),
              (*bullet)->getPosition(), (*bullet)->getVelocity(),
              (*bullet)->getImage(), (*bullet)->getAngle(),
              (*bullet)->getRotSpd());
            
            sprite->explode();
            explosions.push_back(sprite);
            player->deactivate(bullet);

            (*enemy)->setHealth((*enemy)->getHealth() - 
                (*bullet)->getDamage());

            if((*enemy)->getHealth() <= 0){
              (*sound)[0];
              sprite = new Sprite((*enemy)->getName(),
                (*enemy)->getPosition(), (*enemy)->getVelocity(),
                (*enemy)->getImage(), (*enemy)->getAngle(),
                (*enemy)->getRotSpd());

              sprite->explode();
              explosions.push_back(sprite);

              player->setScore(player->getScore() + 1);
              Spawner::curEnemies--;

              delEnemy = true;
              player->detach(*enemy);
              delete *enemy;
              enemy = enemies.erase(enemy);
            }

            break;
        }

        bullet++;
      }
    }

    if(!delEnemy) enemy++;
    else delEnemy = false;
  }

}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if(event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if(keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]){
          done = true;
          break;
        }

        if(keystate[SDL_SCANCODE_P]){
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }

        if(keystate[SDL_SCANCODE_G]) player->toggleGod();
        if(keystate[SDL_SCANCODE_LSHIFT]) player->swapWeapon();
        if(keystate[SDL_SCANCODE_F1]) hud->toggle();

        if(keystate[SDL_SCANCODE_F4] && !makeVideo){
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }

        else if(keystate[SDL_SCANCODE_F4] && makeVideo){
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if(ticks > 0){
      clock.incrFrame();

      if(keystate[SDL_SCANCODE_W]) player->up();
      if(keystate[SDL_SCANCODE_S]) player->down();
      if(keystate[SDL_SCANCODE_A]) player->left();
      if(keystate[SDL_SCANCODE_D]) player->right();
      if(keystate[SDL_SCANCODE_LEFT]) player->wpnClockwise();
      if(keystate[SDL_SCANCODE_RIGHT]) player->wpnCounterClockwise();
      if(keystate[SDL_SCANCODE_SPACE]) player->shoot();

      draw();
      update(ticks);
      if(makeVideo){
        frameGen.makeFrame();
      }
    }
  }
}
