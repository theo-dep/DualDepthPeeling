#include "Geom/Point.h"
#include "Geom/Vector.h"

#include <QtCore/QDebug>

namespace geom
{
    // CONSTRUCTORS

    //----------------------------------------------------------------------------------------------------------------
    inline Plane::Plane() : m_plane(1, 0, 0, 0)
    //----------------------------------------------------------------------------------------------------------------
    {
    }

    //----------------------------------------------------------------------------------------------------------------
    inline Plane::Plane(const Plane& p_plane) : m_plane(p_plane.m_plane)
    //----------------------------------------------------------------------------------------------------------------
    {
    }

    //----------------------------------------------------------------------------------------------------------------
    inline Plane::Plane(double a, double b, double c, double d, bool *isOk /*=nullptr*/) : Plane()
    //----------------------------------------------------------------------------------------------------------------
    {
        bool success = set(a, b, c, d);

        // Check if isOk is defined, no its value
        if (isOk)
        {
            *isOk = success;
        }

        // On Failure, add log Entry
        if (!success)
        {
            qCritical() << "Set failed. Plane defaults to YOZ()";
        }
    }

    //----------------------------------------------------------------------------------------------------------------
    inline Plane::Plane(const Vector& p_normal, const Point& p_point, bool *isOk /*=nullptr*/) : Plane()
    //----------------------------------------------------------------------------------------------------------------
    {
        bool success = set(p_normal, p_point);

        // Check if isOk is defined, no its value
        if (isOk)
        {
            *isOk = success;
        }

        // On Failure, add log Entry
        if (!success)
        {
            qCritical() << "Set failed. Plane defaults to YOZ()";
        }
    }

    //----------------------------------------------------------------------------------------------------------------
    inline Plane::Plane(const Point& p_point1, const Point& p_point2, const Point& p_point3, bool* isOk /*= nullptr*/) : Plane()
    //----------------------------------------------------------------------------------------------------------------
    {
        bool success = set(p_point1, p_point2, p_point3);

        // Check if isOk is defined, no its value
        if (isOk)
        {
            *isOk = success;
        }

        // On Failure, add log Entry
        if (!success)
        {
            qCritical() << "Set failed. Plane defaults to YOZ()";
        }
    }

    // MUTATORS

    //--------------------------------------------------------------------------------------------------------------
    inline bool Plane::set(double p_a, double p_b, double p_c, double p_d)
    //--------------------------------------------------------------------------------------------------------------
    {
        if (p_a == 0 && p_b == 0 && p_c == 0)
        {
            qCritical() << "Set failed. Vector(a,b,c) is null";
            return false;
        }

        m_plane.set(p_a, p_b, p_c, p_d);

        // Normalize
        m_plane *= 1 / Vector(p_a, p_b, p_c).length();

        return true;
    }

    //--------------------------------------------------------------------------------------------------------------
    inline bool Plane::set(const Point& p_point1, const Point& p_point2, const Point& p_point3)
    //--------------------------------------------------------------------------------------------------------------
    {
        const Vector v1 = p_point2 - p_point1;
        const Vector v2 = p_point3 - p_point1;

        // Check that vector are not collinear
        //if (v1.isCollinear(v2, 0))
        //{
        //    QCritical() << QString("Set failed. Points are aligned.");
        //    return false;
        //}

        // v1 and v2 are not collinear and not null (it would have been caught in the isCollinear() check) so their cross product is not null either
        const Vector normal = v1^v2;

        return set(normal, p_point1);
    }

    //----------------------------------------------------------------------------------------------------------------
    inline bool Plane::set(const Vector& p_normal, const Point& p_point)
    //----------------------------------------------------------------------------------------------------------------
    {
        //if (p_normal.isNull(0))
        //{
        //    QCritical() << "Cannot construct plane with null normal";
        //    return false;
        //}

        // By definition, normal vector is (a,b,c)
        // d is computed using p_point. Since it belongs to the plane, we have a*p_x + b*p_y + c*p_z + d = 0
        m_plane.set(p_normal.x(), p_normal.y(), p_normal.z(), -(p_normal.x() * p_point.x() + p_normal.y() * p_point.y() + p_normal.z() * p_point.z()));

        // Normalize
        m_plane *= 1 / p_normal.length();

        return true;
    }

    // ACCESSORS

    //----------------------------------------------------------------------------------------------------------------
    inline double Plane::a() const
    //----------------------------------------------------------------------------------------------------------------
    {
        return m_plane.at(0);
    }

    //----------------------------------------------------------------------------------------------------------------
    inline double Plane::b() const
    //----------------------------------------------------------------------------------------------------------------
    {
        return m_plane.at(1);
    }

    //----------------------------------------------------------------------------------------------------------------
    inline double Plane::c() const
    //----------------------------------------------------------------------------------------------------------------
    {
        return m_plane.at(2);
    }

    //----------------------------------------------------------------------------------------------------------------
    inline double Plane::d() const
    //----------------------------------------------------------------------------------------------------------------
    {
        return m_plane.at(3);
    }

    //----------------------------------------------------------------------------------------------------------------
    inline Point Plane::origin() const
    //----------------------------------------------------------------------------------------------------------------
    {
        return project(Point::ORIGIN());
    }

    //----------------------------------------------------------------------------------------------------------------
    inline Vector Plane::normal() const
    //----------------------------------------------------------------------------------------------------------------
    {
        return Vector(a(), b(), c());
    }

    // COMPUTERS

    //-----------------------------------------------------------------------------------
    inline Point Plane::project(const Point& p_point) const
    //-----------------------------------------------------------------------------------
    {
        // Hard Coded Point projection
        // Based on vgl_closest_point.txx (Point on a plane that is the closest to p0 is the orthogonal projection of p0 on the plane)
        const double a2 = a() * a();
        const double b2 = b() * b();
        const double c2 = c() * c();
        const double ab = a() * b();
        const double ac = a() * c();
        const double bc = b() * c();
        const double e = 1.0 / (a2 + b2 + c2);
        return Point(((b2 + c2) * p_point.x() - ab * p_point.y() - ac * p_point.z() - a() * d()) * e,
                     ((a2 + c2) * p_point.y() - ab * p_point.x() - bc * p_point.z() - b() * d()) * e,
                     ((a2 + b2) * p_point.z() - ac * p_point.x() - bc * p_point.y() - c() * d()) * e);
    }

    //-----------------------------------------------------------------------------------
    inline Vector Plane::project(const Vector& p_vector) const
    //-----------------------------------------------------------------------------------
    {
        return project(origin() + p_vector) - origin();
    }

    //----------------------------------------------------------------------------------------------------------------
    inline Plane Plane::operator-() const
    //----------------------------------------------------------------------------------------------------------------
    {
        const Plane p(-a(), -b(), -c(), -d());
        return p;
    }

    //----------------------------------------------------------------------------------------------------------------
    inline Plane& Plane::flip()
    //----------------------------------------------------------------------------------------------------------------
    {
        // Update each coefficient with its opposite
        // The plane is the same, except that its normal is in the opposite direction
        m_plane.set(-a(), -b(), -c(), -d());
        return (*this);
    }

    //----------------------------------------------------------------------------------------------------------------
    inline void Plane::getBase(Vector& p_i, Vector& p_j) const
    //----------------------------------------------------------------------------------------------------------------
    {
        // p_i is computed as the vector with the highest length among projection of Vector::I(), Vector::J() and Vector::K() on this Plane
        // p_j is computed as normal()^p_i
        Vector
            projI(project(Vector::I())),
            projJ(project(Vector::J())),
            projK(project(Vector::K()));

        Vector* i = &projI;

        if (i->squaredLength() < projJ.squaredLength()) i = &projJ;
        if (i->squaredLength() < projK.squaredLength()) i = &projK;

        i->normalize();
        p_i = *i;
        p_j = normal() ^ p_i;
    }

    // OPERATORS

    //-----------------------------------------------------------------------------------
    inline bool Plane::operator==(const Plane& p_other) const
    //-----------------------------------------------------------------------------------
    {
        return (m_plane == p_other.m_plane);
    }

    //-----------------------------------------------------------------------------------
    inline bool Plane::operator!=(const Plane& p_other) const
    //-----------------------------------------------------------------------------------
    {
        return (m_plane != p_other.m_plane);
    }

    //----------------------------------------------------------------------------------------------------------------
    inline Plane& Plane::operator=(const Plane& p_other)
    //----------------------------------------------------------------------------------------------------------------
    {
        m_plane = p_other.m_plane;
        return (*this);
    }
}
