#include <string>
#include <vector>
#include <map>
#include "sprite.h"

class Viewport;

class Hud : public Sprite {

  public:
    Hud(const std::string&);
    virtual ~Hud();

    void toggle() { enabled = !enabled; }
    bool isEnabled() { return enabled; }

    void setShield(int);
    void setHealth(int);
    void setScore(int);
    void setWeapon(std::string n){ plrWeapon = n; }

    virtual void draw();

  private:
    std::map<std::string, std::string> fields;
    std::map<std::string, std::string> essentials;

    std::vector<Drawable*> shield;
    std::vector<Drawable*> health;
    std::vector<Drawable*> score;

    int plrShield;
    int plrHealth;
    int plrScore;
    std::string plrWeapon;

    int padding;
    bool enabled;

};
