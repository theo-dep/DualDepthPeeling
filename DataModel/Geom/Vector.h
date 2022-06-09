#pragma once

/// @file Vector.h
/// @brief Contains declaration of class gps::geom::Vector
/// @ingroup geom

#include "Geom/Vec4.h"

namespace geom
{
    class Point;

    /// @addtogroup geom
    /// @{

    /// @class Vector Vector.h "GPS/Geom/Vector.h"
    /// @brief Class describing a vector of 3d space

    class Vector
    {
        /// @cond PRIVATE
        friend class Point;
        /// @endcond

        // This method is not a member of Vec4 because the common use is to write p_scalar*p_vector.
        /// @brief Multiplies a Vector by a scalar.
        /// @details This method multiply each coefficient of the provided Vector by the provided scalar.
        /// @param [in] p_scalar: Scalar to use for multiplication.
        /// @param [in] p_vector: Vector to multiply.
        /// @return A Vector containing the result of the multiplication.
        friend Vector operator*(double p_scalar, const Vector& p_vector);

    public:
        /// @brief Constant null Vector.
        static const Vector& ZERO();
        /// @brief Constant unit Vector in x direction.
        static const Vector& I();
        /// @brief Constant unit Vector in y direction.
        static const Vector& J();
        /// @brief Constant unit Vector in z direction.
        static const Vector& K();

        // CONSTRUCTORS

        /// @brief Default constructor
        /// @details This initializes the Vector to Vector::ZERO()
        Vector();

        /// @brief Copy constructor
        /// @details This is initialized using the attributes of @a p_vec
        Vector(const Vector& p_vec);

        /// @brief Constructor using 3 doubles (Vector coordinates)
        /// @note p_z is optional. Default value is 0
        explicit Vector(double p_x, double p_y, double p_z = 0.0);

        /// @brief Constructor using 2 @link Point Points @endlink
        /// @note Vector is oriented from @a p_point1 to @a p_point2
        explicit Vector(const Point& p_point1, const Point& p_point2);

        // MUTATORS

        /// @brief Updates the Vector coordinates (x,y,z).
        /// @note p_z is optional. Default value is 0
        void set(double p_x, double p_y, double p_z = 0.0);

        /// @brief Updates the Vector with the two @link Point Points @endlink provided.
        /// @note Vector is oriented from @a p_point1 to @a p_point2
        void set(const Point& p_point1, const Point& p_point2);

        /// @brief Sets the x-component
        void x(double p_x);

        /// @brief Sets the y-component
        void y(double p_y);

        /// @brief Sets the z-component
        void z(double p_z);

        // ACCESSORS

        /// @brief Gets the x-component
        double x() const;

        /// @brief Gets the y-component
        double y() const;

        /// @brief Gets the z-component
        double z() const;

        /// @brief Gets the normalized vector pointing to the same direction
        /// @details If @a this is null, Vector::ZERO() is returned
        Vector normalized() const;

        /// @brief Gets the length of the Vector.
        double length() const;

        /// @brief Gets the square length of the Vector.
        double squaredLength() const;

        // COMPUTERS

        /// @brief Adds the provided values to the Vector coordinates.
        /// @param [in] dx: Value to add to x coordinate
        /// @param [in] dy: Value to add to y coordinate
        /// @param [in] dz: Value to add to z coordinate
        /// @return Vector result of the operation.
        Vector add(double dx, double dy, double dz) const;

        /// @brief Multiplies the Vector coordinates by the provided values.
        /// @param [in] mx: Factor to apply on x coordinate
        /// @param [in] my: Factor to apply on y coordinate
        /// @param [in] mz: Factor to apply on z coordinate
        /// @return Vector result of the operation
        Vector mul(double mx, double my, double mz) const;

        /// @brief In-place normalization
        /// @details If @a this is not null, it is divided by its @link length() length @endlink.\n
        /// If it is null, it is unchanged and method return false
        /// @return True on success
        bool normalize();

        // OPERATORS

        /// @brief Vector strict equality
        /// @details This method checks that the coordinates of each @link Vector Vectors @endlink are strictly equals.
        /// @return True if strictly equal
        /// @note This function should not be used to test geometric equality, prefer to use equals() function instead.
        bool operator==(const Vector& p_other) const;

        /// @brief Inequality operator
        /// @return True if @link Vector Vectors @endlink are different
        /// @sa operator==()
        bool operator!=(const Vector& p_other) const;

        /// @brief Gets the opposite Vector
        Vector operator-() const;

        /// @brief Affectation operator
        /// @details Update the attribute of this Vector with the attribute of @a p_other.
        /// @return A reference to this Vector after being updated.
        Vector& operator=(const Vector& p_vec);

        /// @brief Addition of two @link Vector Vectors @endlink
        /// @return @a this + @a p_other
        Vector operator+(const Vector& p_other) const;

        /// @brief Subtraction of two @link Vector Vectors @endlink
        /// @return @a this - @a p_other
        Vector operator-(const Vector& p_other) const;

        /// @brief In-place addition
        /// @details Sets the result of @a this + @a p_vec to @a this
        /// @return A reference to this Vector after being updated.
        Vector& operator+=(const Vector& p_vec);

        /// @brief In-place subtraction
        /// @details Sets the result of @a this - @a p_vec to @a this
        /// @return A reference to this Vector after being updated.
        Vector& operator-=(const Vector& p_vec);

        /// @brief Dot product of two @link Vector Vectors @endlink
        /// @return this.p_other
        double operator*(const Vector& p_other) const;

        /// @brief Multiplies by a scalar.
        /// @return @a this * @a factor
        Vector operator*(double p_factor) const;

        /// @brief In-place multiplication by a scalar
        /// @return A reference to this Vector after being updated.
        Vector& operator*=(double p_factor);

        /// @brief Divides by a scalar.
        /// @return @a this / @a factor
        /// @warning If @a p_factor is 0, Vector returned is the same as input vector.
        Vector operator/(double p_factor) const;

        /// @brief In-place scalar division
        /// @return A reference to this Vector after being updated.
        /// @warning If @a p_factor is 0, division the Vector is left unchanged
        Vector& operator/=(double p_factor);

        /// @brief Cross product of two @link Vector Vectors @endlink
        /// @return @ this ^ @a p_other
        Vector operator^(const Vector& p_other) const;

    private:
        /// @link Vec4 Vector of double @endlink containing the vector coordinates
        /// @note Last coordinate is always 0 (Infinite Point in homogeneous coordinates)
        Vec4 m_vec;
    };

    /// @}
}

/// @cond PRIVATE
#include "Geom/Vector.inl.cpp"
/// @endcond
