#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char* args[]) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Can't init sdl2: " << SDL_GetError() << std::endl;
        return 1;
  }

  const auto window = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_UTILITY);
  if (window == nullptr){
      std::cerr << "Can't create window: " << SDL_GetError() << std::endl;
      SDL_Quit();
      return 1;
  }

  const auto screenSurface = SDL_GetWindowSurface(window);
  if (screenSurface == nullptr){
      std::cerr << "Can't get screen surface: " << SDL_GetError() << std::endl;
      SDL_Quit();
      return 1;
  }
  SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0x00));
  SDL_UpdateWindowSurface(window);
  SDL_Delay(5000);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
