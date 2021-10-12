#include "../Scene.h"
#include "../Engine.h"

namespace RGSDL {
  
    Scene::Scene(bool p) : persistent(p), started(false) {}

    bool Scene::onStart(Engine *game) { return true; }
    Scene* Scene::onEnd(Engine *game) { return nullptr; }

}
