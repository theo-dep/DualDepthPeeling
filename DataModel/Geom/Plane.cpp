#include "Geom/Plane.h"

namespace geom
{
    //-------------------------------------------------------------------------------------------------------------------
    /*static*/ const Plane& Plane::XOY ()
    //-------------------------------------------------------------------------------------------------------------------
    {
        static const Plane s_XOY(0, 0, 1, 0);
        return s_XOY;
    }

    //-------------------------------------------------------------------------------------------------------------------
    /*static*/ const Plane& Plane::YOX()
    //-------------------------------------------------------------------------------------------------------------------
    {
        static const Plane s_YOX(0, 0, -1, 0);
        return s_YOX;
    }

    //-------------------------------------------------------------------------------------------------------------------
    /*static*/ const Plane& Plane::XOZ()
    //-------------------------------------------------------------------------------------------------------------------
    {
        static Plane s_XOZ(0, -1, 0, 0);
        return s_XOZ;
    }

    //-------------------------------------------------------------------------------------------------------------------
    /*static*/ const Plane& Plane::ZOX()
    //-------------------------------------------------------------------------------------------------------------------
    {
        static Plane s_ZOX(0, 1, 0, 0);
        return s_ZOX;
    }
    //-------------------------------------------------------------------------------------------------------------------
    /*static*/ const Plane& Plane::YOZ()
    //-------------------------------------------------------------------------------------------------------------------
    {
        static Plane s_YOZ(1, 0, 0, 0);
        return s_YOZ;
    }

    //-------------------------------------------------------------------------------------------------------------------
    /*static*/ const Plane& Plane::ZOY()
    //-------------------------------------------------------------------------------------------------------------------
    {
        static Plane s_ZOY(-1, 0, 0, 0);
        return s_ZOY;
    }
}