# RGSDL
An attempt to create "GameEngine" like Framework around SDL2

To see what features are available check the .h - files in the src/engine folder.

### Available functions
- Processing inputs for keyboard and mouse 
- Providing a `deltaTime` modifier for Framerate independed processing
- Provides a "Transform" - component, that allows for easy manipulation of rotations, scales and locations.
- Loading textures for spritesheets and images (right now only .png files are supported)
- Creation of tilemaps based on sprite-/tilesheet textures

### How to use it:
- put the `src/engine` folder into your project
- in your `main` function, you can use the engine as follows:
```cpp
#include "./engine/Engine.h"
#include "./engine/Scene.h"

#include "./MainScene.h"
// You need to provide your own
// have a look at the example project to get an idea what a Scene looks like.

class MainScene  {}

int main(int argc, char** argv)

  MainScene ms = MainScene();

  Engine game;

  // 256 x 192 at 96dpi = NintendoDSi XL Screensize
  // The engine will scale the window so that is will take roughly 4 to 5 inches on your current screen
  return game.start( argc, argv, 256, 192, 96.0f, "Example-Project", &ms);

}

```

### How to compile it:
- Make sure your compiler has access to SDL2 and SDL2_image (`-lSDL2 -lSDL2main -lSDL2_image`) \
- Put the `SDL2.dll`, `SDL2_image.dll` as well as the other required DLLs into the same folder, where your compiled binary will be.

- If you use linux or msys, then you can use the Makefile in this project\
to compile it.
  - use `make` to create a debug build.
  - use `make run` to create and start the debug build.
  - use `make release` to create a build, that does not open a terminal in the background and requires less ressources to run.
  - use `make clean` to remove all compiled files

### TODO:
- ⬜ Create a "draw" - Layer, where SDL_Render draw functions can be used.
- ⬜ Implement the Global-System (from [WASM_WASteroids](https://github.com/DoodlingTurtle/WASM_WAsteroids))
- ...
- ⬜ Documentation 

## DONE:
- ✔ Implement the Scene-System (from [WASM_WASteroids](https://github.com/DoodlingTurtle/WASM_WAsteroids))
- ✔ Create SDL-Window and Close-Event
- ✔ Make window resizeable
- ✔ Reset the screen and border color in various colors
- ✔ Learn to create and draw 2d textures (pictures)
- ✔ Keyboard input
- ✔ MouseInput
- ✔ combine Textures with Transforms
- ✔ implement zDepth to define drawing prioritys
- ✔ Learn to draw partial textures
- ✔ create a tilemap system 
