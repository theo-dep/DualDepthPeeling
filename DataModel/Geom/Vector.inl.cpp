#include "Geom/Point.h"

#include <QtCore/QtDebug>

namespace geom
{

    //-------------------------------------------------------------------------------------------------------------------
    inline Vector::Vector() : m_vec(0, 0, 0, 0)
    //-------------------------------------------------------------------------------------------------------------------
    {
    }

    //-------------------------------------------------------------------------------------------------------------------
    inline Vector::Vector(double p_x, double p_y, double p_z /*=0*/) : m_vec(p_x, p_y, p_z, 0)
    //-------------------------------------------------------------------------------------------------------------------
    {
    }

    //--------------------------------------------------------------------------------------------------------------
    inline Vector::Vector(const Point& p_point1, const Point& p_point2)
    //--------------------------------------------------------------------------------------------------------------
    {
        set(p_point1, p_point2);
    }

    //--------------------------------------------------------------------------------------------------------------
    inline Vector::Vector(const Vector& p_vec) : m_vec(p_vec.m_vec)
    //--------------------------------------------------------------------------------------------------------------
    {
    }

    //--------------------------------------------------------------------------------------------------------------
    inline void Vector::set(double p_x, double p_y, double p_z)
    //--------------------------------------------------------------------------------------------------------------
    {
        m_vec.set(p_x, p_y, p_z, 0);
    }

    //--------------------------------------------------------------------------------------------------------------
    inline void Vector::set(const Point& p_point1, const Point& p_point2)
    //--------------------------------------------------------------------------------------------------------------
    {
        m_vec = p_point2.m_point - p_point1.m_point;
    }

    //--------------------------------------------------------------------------------------------------------------
    inline void Vector::x(double p_x)
    //--------------------------------------------------------------------------------------------------------------
    {
        m_vec.at(0) = p_x;
    }

    //--------------------------------------------------------------------------------------------------------------
    inline void Vector::y(double p_y)
    //--------------------------------------------------------------------------------------------------------------
    {
        m_vec.at(1) = p_y;
    }

    //--------------------------------------------------------------------------------------------------------------
    inline void Vector::z(double p_z)
    //--------------------------------------------------------------------------------------------------------------
    {
        m_vec.at(2) = p_z;
    }

    //--------------------------------------------------------------------------------------------------------------
    inline double Vector::x() const
    //--------------------------------------------------------------------------------------------------------------
    {
        return m_vec.at(0);
    }

    //--------------------------------------------------------------------------------------------------------------
    inline double Vector::y() const
    //--------------------------------------------------------------------------------------------------------------
    {
        return m_vec.at(1);
    }

    //--------------------------------------------------------------------------------------------------------------
    inline double Vector::z() const
    //--------------------------------------------------------------------------------------------------------------
    {
        return m_vec.at(2);
    }

    //-------------------------------------------------------------------------------------------------------------------
    inline Vector Vector::add(double dx, double dy, double dz) const
    //-------------------------------------------------------------------------------------------------------------------
    {
        return Vector(x() + dx, y() + dy, z() + dz);
    }

    //-------------------------------------------------------------------------------------------------------------------
    inline Vector Vector::mul(double mx, double my, double mz) const
    //-------------------------------------------------------------------------------------------------------------------
    {
        return Vector(x() * mx, y() * my, z() * mz);
    }

    //--------------------------------------------------------------------------------------------------------------
    inline Vector& Vector::operator+=(const Vector& p_vec)
    //--------------------------------------------------------------------------------------------------------------
    {
        m_vec += p_vec.m_vec;
        return (*this);
    }

    //--------------------------------------------------------------------------------------------------------------
    inline Vector& Vector::operator-=(const Vector& p_vec)
    //--------------------------------------------------------------------------------------------------------------
    {
        m_vec -= p_vec.m_vec;
        return *this;
    }

    //--------------------------------------------------------------------------------------------------------------
    inline Vector& Vector::operator*=(double p_factor)
    //--------------------------------------------------------------------------------------------------------------
    {
        m_vec *= p_factor;
        return (*this);
    }

    //--------------------------------------------------------------------------------------------------------------
    inline Vector Vector::operator/(double p_factor) const
        //--------------------------------------------------------------------------------------------------------------
    {
        if (p_factor == 0)
        {
            qCritical() << "Attempt to divide by 0. Returning input vector";
            return Vector(*this);
        }
        else
        {
            const double invFactor = 1.0 / p_factor;
            const Vector v(x() * invFactor, y() * invFactor, z() * invFactor);
            return v;
        }
    }

    //--------------------------------------------------------------------------------------------------------------
    inline Vector& Vector::operator/=(double p_factor)
    //--------------------------------------------------------------------------------------------------------------
    {
        if (p_factor == 0)
        {
            qCritical() << "Attempt to divide by 0. Vector is unchanged";
            return (*this);
        }
        else
        {
            double invFactor = 1.0 / p_factor;
            m_vec.at(0) = m_vec.at(0) * invFactor;
            m_vec.at(1) = m_vec.at(1) * invFactor;
            m_vec.at(2) = m_vec.at(2) * invFactor;
            return (*this);
        }
    }

    //--------------------------------------------------------------------------------------------------------------
    inline double Vector::length() const
    //--------------------------------------------------------------------------------------------------------------
    {
        return m_vec.length();
    }

    //--------------------------------------------------------------------------------------------------------------
    inline double Vector::squaredLength() const
    //--------------------------------------------------------------------------------------------------------------
    {
        return m_vec.squaredLength();
    }

    //--------------------------------------------------------------------------------------------------------------
    inline Vector Vector::operator-() const
    //--------------------------------------------------------------------------------------------------------------
    {
        return Vector(-x(), -y(), -z());
    }

    //-------------------------------------------------------------------------------------------------------------------
    inline bool Vector::normalize()
    //-------------------------------------------------------------------------------------------------------------------
    {
        const double d = m_vec.length();

        if (d != 0)
        {
            const double invd = 1.0 / d;
            m_vec.at(0) = m_vec.at(0) * invd;
            m_vec.at(1) = m_vec.at(1) * invd;
            m_vec.at(2) = m_vec.at(2) * invd;
            return true;
        }
        else
        {
#ifdef _DEBUG
            qCritical() << "Cannot normalize null vector. Vector is unchanged.";
#endif
            return false;
        }
    }

    //-------------------------------------------------------------------------------------------------------------------
    inline Vector Vector::normalized() const
    //-------------------------------------------------------------------------------------------------------------------
    {
        Vector v(*this);
        v.normalize();
        return v;
    }

    //// OPERATORS

    //-----------------------------------------------------------------------------------
    inline Vector Vector::operator-(const Vector& p_other) const
    //-----------------------------------------------------------------------------------
    {
        Vector v;
        v.m_vec = m_vec - p_other.m_vec;
        return  v;
    }

    //-----------------------------------------------------------------------------------
    inline Vector Vector::operator+(const Vector& p_other) const
    //-----------------------------------------------------------------------------------
    {
        Vector v;
        v.m_vec = m_vec + p_other.m_vec;
        return  v;
    }

    //-----------------------------------------------------------------------------------
    inline /*friend*/ Vector operator*(double p_scalar, const Vector& p_vector)
    //-----------------------------------------------------------------------------------
    {
        Vector v;
        v.m_vec = p_scalar * p_vector.m_vec;
        return v;
    }

    //-----------------------------------------------------------------------------------
    inline double Vector::operator*(const Vector& p_other) const
    //-----------------------------------------------------------------------------------
    {
        return m_vec * p_other.m_vec;
    }

    //--------------------------------------------------------------------------------------------------------------
    inline Vector Vector::operator*(double p_factor) const
    //--------------------------------------------------------------------------------------------------------------
    {
        const Vector v(x() * p_factor, y() * p_factor, z() * p_factor);
        return v;
    }

    //-----------------------------------------------------------------------------------
    inline Vector Vector::operator^(const Vector& p_vector2) const
    //-----------------------------------------------------------------------------------
    {
        // Cross Product is written here because there is no generic cross product for 4D Vectors
        return Vector(y() * p_vector2.z() - z() * p_vector2.y(),
            z() * p_vector2.x() - x() * p_vector2.z(),
            x() * p_vector2.y() - y() * p_vector2.x());
    }

    //-----------------------------------------------------------------------------------
    inline bool Vector::operator==(const Vector& p_other) const
    //-----------------------------------------------------------------------------------
    {
        return (m_vec == p_other.m_vec);
    }

    //-----------------------------------------------------------------------------------
    inline bool Vector::operator!=(const Vector& p_other) const
    //-----------------------------------------------------------------------------------
    {
        return (m_vec != p_other.m_vec);
    }

    //-----------------------------------------------------------------------------------
    inline Vector& Vector::operator=(const Vector& p_vec)
    //-----------------------------------------------------------------------------------
    {
        m_vec = p_vec.m_vec;
        return (*this);
    }
}