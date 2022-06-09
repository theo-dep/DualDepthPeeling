#pragma once

/// @file Point.h
/// @brief Contains the declaration of class gps::geom::Point
/// @ingroup geom

#include "Geom/Vec4.h"

namespace geom
{
    class Vector;

    /// @addtogroup geom
    /// @{

    /// @class Point Point.h "GPS/Geom/Point.h"
    /// @brief Class describing a point of the 3d affine space.

    class Point
    {
        /// @cond PRIVATE
        friend class Vector;
        /// @endcond

    public:
        /// @brief Constant Point(0,0,0).
        static const Point& ORIGIN();

        // CONSTRUCTORS

        /// @brief Default constructor
        /// @details This initializes the Point to Point::ORIGIN()
        Point();

        /// @brief Copy constructor
        /// @details This is initialized using the attributes of @a p_point
        Point(const Point& p_point);

        /// @brief Constructor using 3 doubles (Point coordinates)
        /// @note p_z is optional. Default value is 0
        Point(double p_x, double p_y, double p_z = 0.0);

        // MUTATORS

        /// @brief Updates the Point coordinates (x,y,z).
        /// @note p_z is optional. Default value is 0
        void set(double p_x, double p_y, double p_z = 0.0);

        /// @brief Sets on the x-component
        void x(double p_x);

        /// @brief Sets on the y-component
        void y(double p_y);

        /// @brief Sets on the z-component
        void z(double p_z);

        // ACCESSORS

        /// @brief Gets the x-component
        double x() const;

        /// @brief Gets the y-component
        double y() const;

        /// @brief Gets the z-component
        double z() const;

        // COMPUTERS

        /// @brief Adds the provided values to the Point coordinates.
        /// @param [in] dx: Value to add to x coordinate
        /// @param [in] dy: Value to add to y coordinate
        /// @param [in] dz: Value to add to z coordinate
        /// @return Point result of the operation.
        Point add(double dx, double dy, double dz) const;

        /// @brief Multiplies the Point coordinates by the provided values.
        /// @param [in] mx: Factor to apply on x coordinate
        /// @param [in] my: Factor to apply on y coordinate
        /// @param [in] mz: Factor to apply on z coordinate
        ///@return Point result of the operation
        Point mul(double mx, double my, double mz) const;

        /// @brief Computes the distance to another Point
        /// @param [in] other: Point to compute distance to.
        /// @return d[@a this, @a other]
        double distance(const Point& other) const;

        /// @brief Computes the squared distance to another Point
        /// @param [in] other: Point to compute distance to.
        /// @return d[this, other] * d[this, other]
        double squaredDistance(const Point& other) const;

        // OPERATORS

        /// @brief Point strict equality.
        /// @details This method checks that the coordinates of each @link Point Points @endlink are strictly equals.
        /// @return True if strictly equal
        /// @note This function should not be used to test geometric equality, prefer to use equals() function instead.
        bool operator==(const Point& p_other) const;

        /// @brief Inequality operator
        /// @return True if @link Point Points @endlink are different
        /// @sa operator==()
        bool operator!=(const Point& p_other) const;

        /// @brief Affectation operator
        /// @details Update the attribute of this Point with the attribute of @a p_other.
        /// @return A reference to this Point after being updated.
        Point& operator=(const Point& p_other);

        /// @brief Affine addition with a Vector
        /// @return @a this + @a p_vector
        Point operator+(const Vector& p_vector) const;

        /// @brief In-place addition with a Vector
        /// @details Sets the result of @a this + @a p_vec to @a this
        /// @return A reference to this Point after being updated.
        Point& operator+=(const Vector& p_vector);

        /// @brief Vector joining two @link Point Points @endlink
        /// @return Vector @a p_other->@a this
        Vector operator-(const Point& p_other) const;

    private:
        /// @brief @link Vec4 Vector @endlink representing the Points coordinates
        /// @note Last coordinate is always 1 (finite Point in homogeneous coordinates)
        Vec4 m_point;


    };

    /// @}
}

/// @cond PRIVATE
#include "Geom/Point.inl.cpp"
/// @endcond
