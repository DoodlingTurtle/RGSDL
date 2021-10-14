#include "../Transform.h"
#include <SDL2/SDL_rect.h>

namespace RGSDL {

    //=============================================================================
    // Angle
    //=========================================================================={{{

    void Angle::_recalc() {
        radiants = (PI2 / 360.0) * angle;
        direction = {cos(radiants), sin(radiants)};
    }

    Angle::Angle(float deg) : angle(deg) { _recalc(); }

    SDL_FPoint Angle::getDirection() const { return SDL_FPoint(direction); }
    float Angle::getRadiants() const { return radiants; }
    float Angle::getDegree() const { return angle; }

#define def_op(_op) const Angle &Angle::operator _op(float deg) { angle _op deg; _recalc(); return *this; }

    def_op(=) def_op(+=) def_op(-=) def_op(*=) def_op(/=) ;

#undef dev_op
    //}}}

    
//=============================================================================
// Transform
//=========================================================================={{{

    Transform Transform::_default = {{0, 0}, {0, 0}, {1.0f, 1.0f}, 0.0f};

//}}}

} // namespace RG2GE
