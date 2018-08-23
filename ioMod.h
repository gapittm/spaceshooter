#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>

class IOmod {

  public:

    static IOmod& getInstance();
    ~IOmod();

    SDL_Texture* readTexture(const std::string& filename);
    SDL_Surface* readSurface(const std::string& filename);
    SDL_Renderer* getRenderer() const { return renderer; }
    
    void writeText(const std::string&, int, int,
      const std::string& = "defaultfont") const;

    void setFont(const std::string& n);
    std::vector<std::pair<std::string, std::pair<TTF_Font*, SDL_Color>>>::const_iterator
      getFont(const std::string& n) const;

  private:

    int init;
    SDL_Renderer* renderer;
    std::vector<std::pair<std::string,
      std::pair<TTF_Font*, SDL_Color>>> fonts;

    IOmod();
    IOmod(const IOmod&) = delete;
    IOmod& operator=(const IOmod&) = delete;

};
