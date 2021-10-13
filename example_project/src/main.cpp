#include <RGSDL/Engine.h>
#include <cstring>

#include "./scenes/Sandbox.h"

using namespace RGSDL;

int main(int argc, char** argv) {

    Scenes::Sandbox scene_sandbox; 
    
    RGSDL::Engine game; 
    return game.start(
            argc, argv,
            256, 192, 103.0f, 
            "RGSDL-Demo", 
            &scene_sandbox
    );
}
