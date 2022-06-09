#include "Geom/Vector.h"

namespace geom
{
    //-------------------------------------------------------------------------------------------------------------------
    const Vector& Vector::ZERO()
    //-------------------------------------------------------------------------------------------------------------------
    {
        static const Vector s_zero(0,0,0);
        return s_zero;
    }

    //-------------------------------------------------------------------------------------------------------------------
    const Vector& Vector::I()
    //-------------------------------------------------------------------------------------------------------------------
    {
        static const Vector s_i(1,0,0);
        return s_i;
    }

    //-------------------------------------------------------------------------------------------------------------------
    const Vector& Vector::J()
    //-------------------------------------------------------------------------------------------------------------------
    {
        static const Vector s_j(0,1,0);
        return s_j;
    }

    //-------------------------------------------------------------------------------------------------------------------
    const Vector& Vector::K()
    //-------------------------------------------------------------------------------------------------------------------
    {
        static const Vector s_k(0,0,1);
        return s_k;
    }
}