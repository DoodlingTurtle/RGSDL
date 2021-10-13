#pragma once

namespace RGSDL {

    class Engine;

    class Scene {
        public:
            Scene(bool persisten=true);
            virtual ~Scene() = default;

            virtual bool onStart(Engine* game);
            virtual bool onUpdate(Engine* game, float deltaTime) = 0;
            virtual void onDraw(Engine* game) = 0;
            virtual Scene* onEnd(Engine* game);

        private:
            bool persistent;
            bool started;

            friend class Engine;
    };

}
