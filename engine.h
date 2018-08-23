#include <vector>
#include <list>
#include <SDL.h>
#include "sound.h"
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "side.h"

class Collision;
class Player;
class Enemy;
class Turret;
class Bullet;
class Hud;

class Engine {

  public:
    Engine();
    ~Engine();
    void play();

  private:
    const RenderContext* rc;
    const IOmod& io;
    Hud* hud;
    Clock& clock;
    SDL_Renderer * const renderer;
    SDLSound* sound;

    int level;
    World world;
    Viewport& viewport;

    Player* player;
    std::vector<Collision*> strategy;
    std::vector<Drawable*> tiles;
    std::vector<Enemy*> enemies;

    std::vector<Turret*> turrets;
    std::list<Bullet*> turretBullets;

    std::vector<Drawable*> explosions;

    bool makeVideo;

    void addTiles(const int);
    void initPlayer();

    void draw() const;
    void update(Uint32);
    void reset();

    void collisionCheck();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

};
