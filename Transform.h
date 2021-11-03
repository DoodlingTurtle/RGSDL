#pragma once

#include "./Utils.h"
#include <SDL2/SDL_rect.h>

namespace RGSDL {

    struct Angle {
        public:
            Angle(float degree = 0);

            SDL_FPoint getDirection() const;
            float getDegree() const;
            float getRadiants() const;

            const Angle& operator = (float ang);
            const Angle& operator += (float ang);
            const Angle& operator -= (float ang);
            const Angle& operator *= (float ang);
            const Angle& operator /= (float ang);

        private:
            float radiants;
            float angle;
            SDL_FPoint direction;
            void _recalc();
    };

    struct Transform {
        SDL_FPoint position;
        SDL_Point origin;
        SDL_FPoint scale;
        Angle rotation;

        static Transform _default;
    };

} // namespace RG2GE
