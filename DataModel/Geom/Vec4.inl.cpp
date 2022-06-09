#include <QtCore/QtDebug>

namespace geom
{

    //---------------------------------------------------------------------------------
    inline Vec4::Vec4() : m_array{0., 0., 0., 0.}
    //---------------------------------------------------------------------------------
    {
    }

    //---------------------------------------------------------------------------------
    inline /*explicit*/ Vec4::Vec4(double a, double b, double c, double d): m_array{a, b, c, d}
    //---------------------------------------------------------------------------------
    {
    }

    //---------------------------------------------------------------------------------
    inline /*explicit*/ Vec4::Vec4(const double p_array[4], bool* isOk /*=nullptr*/) : Vec4()
    //---------------------------------------------------------------------------------
    {
        bool result = set(p_array);
        if (isOk)
        {
            *isOk = result;
        }
    }

    //---------------------------------------------------------------------------------
    inline Vec4::Vec4(const Vec4& other): m_array{ other.m_array[0], other.m_array[1], other.m_array[2], other.m_array[3] }
    //---------------------------------------------------------------------------------
    {
    }

    //---------------------------------------------------------------------------------
    inline void Vec4::set(double a, double b, double c, double d)
    //---------------------------------------------------------------------------------
    {
        m_array[0] = a;
        m_array[1] = b;
        m_array[2] = c;
        m_array[3] = d;
    }

    //---------------------------------------------------------------------------------
    inline bool Vec4::set(const double p_array[4])
    //---------------------------------------------------------------------------------
    {
        if (p_array != nullptr)
        {
            set(p_array[0], p_array[1], p_array[2], p_array[3]);
            return true;
        }
        else
        {

            qCritical() << "Set Failed. Provided array pointer is null. Vec4 is unchanged.";
            return false;
        }
    }


    //---------------------------------------------------------------------------------
    inline void Vec4::fill(double value)
    //---------------------------------------------------------------------------------
    {
        set(value, value, value, value);
    }

    //---------------------------------------------------------------------------------
    inline bool Vec4::set(int idx, double value)
    //---------------------------------------------------------------------------------
    {
        if (idx < 4 && idx > -1)
        {
            m_array[idx] = value;
            return true;
        }
        else
        {
            qCritical() << QString("Set Failed. Index out of bounds. Vec4 is unchanged. idx:%0").arg(idx);
            return false;
        }
    }

    //---------------------------------------------------------------------------------
    inline double& Vec4::at(int idx)
        //---------------------------------------------------------------------------------
    {
        return m_array[idx];
    }

    //---------------------------------------------------------------------------------
    inline double Vec4::at(int idx) const
    //---------------------------------------------------------------------------------
    {
        return m_array[idx];
    }

    //---------------------------------------------------------------------------------
    inline double Vec4::length() const
    //---------------------------------------------------------------------------------
    {
        return sqrt(squaredLength());
    }

    /// @brief Raises a number to the power 2
    /// @param [in] p_x: Number
    /// @return  p_x^2
    /// @note Provided for convenience
    //---------------------------------------------------------------------------------
    inline double sqr(double p_x)
    //---------------------------------------------------------------------------------
    {
        return p_x * p_x;
    }

    //---------------------------------------------------------------------------------
    inline double Vec4::squaredLength() const
    //---------------------------------------------------------------------------------
    {
        return sqr(m_array[0]) + sqr(m_array[1]) + sqr(m_array[2]) + sqr(m_array[3]);
    }

    //---------------------------------------------------------------------------------
    inline Vec4 Vec4::operator+(const Vec4& other) const
    //---------------------------------------------------------------------------------
    {
        return Vec4(m_array[0] + other.m_array[0],
            m_array[1] + other.m_array[1],
            m_array[2] + other.m_array[2],
            m_array[3] + other.m_array[3]);
    }


    //---------------------------------------------------------------------------------
    inline Vec4 Vec4::operator-(const Vec4& other) const
    //---------------------------------------------------------------------------------
    {
        return Vec4(m_array[0] - other.m_array[0],
            m_array[1] - other.m_array[1],
            m_array[2] - other.m_array[2],
            m_array[3] - other.m_array[3]);
    }

    //---------------------------------------------------------------------------------
    inline double Vec4::operator*(const Vec4& other) const
    //---------------------------------------------------------------------------------
    {
        return m_array[0] * other.m_array[0] +
            m_array[1] * other.m_array[1] +
            m_array[2] * other.m_array[2] +
            m_array[3] * other.m_array[3];
    }

    //---------------------------------------------------------------------------------
    inline Vec4 Vec4::operator*(double factor) const
    //---------------------------------------------------------------------------------
    {
        return Vec4(factor * m_array[0],
            factor * m_array[1],
            factor * m_array[2],
            factor * m_array[3]);
    }

    //---------------------------------------------------------------------------------
    inline Vec4 Vec4::operator-() const
    //---------------------------------------------------------------------------------
    {
        return Vec4(- m_array[0], - m_array[1], - m_array[2], - m_array[3]);
    }

    //---------------------------------------------------------------------------------
    inline double Vec4::operator[](int idx) const
    //---------------------------------------------------------------------------------
    {
        if (idx < 4 && idx > -1)
        {
            return m_array[idx];
        }
        else
        {
            qCritical() << QString("Access Failed. Index out of bounds. Return nan(\"\"). idx:%0").arg(idx);
            return nan("");
        }
    }

    //---------------------------------------------------------------------------------
    inline Vec4& Vec4::operator+=(const Vec4& other)
    //---------------------------------------------------------------------------------
    {
        m_array[0] += other.m_array[0];
        m_array[1] += other.m_array[1];
        m_array[2] += other.m_array[2];
        m_array[3] += other.m_array[3];
        return (*this);
    }

    //---------------------------------------------------------------------------------
    inline Vec4& Vec4::operator-=(const Vec4& other)
    //---------------------------------------------------------------------------------
    {
        m_array[0] -= other.m_array[0];
        m_array[1] -= other.m_array[1];
        m_array[2] -= other.m_array[2];
        m_array[3] -= other.m_array[3];
        return (*this);
    }

    //---------------------------------------------------------------------------------
    inline Vec4& Vec4::operator*=(double factor)
    //---------------------------------------------------------------------------------
    {
        m_array[0] *= factor;
        m_array[1] *= factor;
        m_array[2] *= factor;
        m_array[3] *= factor;
        return (*this);
    }

    //---------------------------------------------------------------------------------
    inline bool Vec4::operator==(const Vec4& other) const
    //---------------------------------------------------------------------------------
    {
        return (m_array[0] == other.m_array[0] && m_array[1] == other.m_array[1] && m_array[2] == other.m_array[2] && m_array[3] == other.m_array[3]);
    }

    //---------------------------------------------------------------------------------
    inline bool Vec4::operator!=(const Vec4& other) const
    //---------------------------------------------------------------------------------
    {
        return (m_array[0] != other.m_array[0] || m_array[1] != other.m_array[1] || m_array[2] != other.m_array[2] || m_array[3] != other.m_array[3]);
    }

    //---------------------------------------------------------------------------------
    inline Vec4& Vec4::operator=(const Vec4& other)
    //---------------------------------------------------------------------------------
    {
        m_array[0] = other.m_array[0];
        m_array[1] = other.m_array[1];
        m_array[2] = other.m_array[2];
        m_array[3] = other.m_array[3];
        return (*this);
    }

    //---------------------------------------------------------------------------------
    inline /*friend*/ Vec4 operator*(double factor, const Vec4& vec)
    //---------------------------------------------------------------------------------
    {
        return Vec4(factor * vec.m_array[0],
            factor * vec.m_array[1],
            factor * vec.m_array[2],
            factor * vec.m_array[3]);
    }
}
