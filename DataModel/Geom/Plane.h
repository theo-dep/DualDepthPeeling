#pragma once

/// @file Plane.h
/// @brief Contains the declaration of class gps::geom::Plane
/// @ingroup geom

#include "Geom/Vec4.h"

namespace geom
{
    class Point;
    class Vector;

    /// @addtogroup geom
    /// @{

    /// @class Plane Plane.h "GPS/Geom/Plane.h"
    /// @brief Class describing a plane of 3d space
    /// @details A plane of coordinates (a,b,c,d) (where (a,b,c) is a non null vector) is a set of points satisfying the equation ax+by+cz+d=0.\n
    /// The coordinates of the plane are normalized when the plane is constructed or the coordinates are updated. \n
    /// The normal vector is accessed using normal(), it is always a unitary vector.
    class Plane
    {
    public:
        /// @brief Plane defined by z=0 with normal oriented along Vector::K()
        static const Plane& XOY ();
        /// @brief Plane defined by z=0 with normal oriented along -1 * Vector::K()
        static const Plane& YOX ();
        /// @brief Plane defined by y=0 with normal oriented along -1 * Vector::J()
        static const Plane& XOZ ();
        /// @brief Plane defined by y=0 with normal oriented along Vector::J()
        static const Plane& ZOX ();
        /// @brief Plane defined by x=0 with normal oriented along Vector::I()
        static const Plane& YOZ ();
        /// @brief Plane defined by x=0 with normal oriented along -1 * Vector::I()
        static const Plane& ZOY ();

        // CONSTRUCTORS

        /// @brief Default constructor
        /// @details This initializes the Plane to Plane::YOZ()
        Plane();

        /// @brief Copy constructor
        /// @details This is initialized using the attributes of @a p_plane
        Plane(const Plane& p_plane);

        /// @brief Constructor using 4 doubles.
        /// @details If vector(a,b,c) is null, Plane defaults to YOZ() and @a isOk is set to false (if provided).\n
        /// On success, coordinates are normalized by the normal vector length
        /// and Plane is defined by equation ax/k + by/k + cz/k + d/k = 0, with k being the normal vector length.
        /// @param [in] a,b,c,d : Coefficients of the plane.
        /// @param [out] isOk: If provided, contains the result of the operation (success or failure).
        /// @warning This constructor does not necessarily build the object defined by the parameters.
        /// @warning If you are not sure of the parameters, call default constructor and then Plane::set(double, double, double, double) instead.
        Plane(double a, double b, double c, double d, bool* isOk = nullptr);

        /// @brief Constructor using a Vector and a Point.
        /// @details If @a p_normal is null, Plane defaults to YOZ() and @a isOk is set to false (if provided).
        /// @param [in] p_normal: Normal Vector of the Plane to build.
        /// @param [in] p_point: Point belonging to the Plane to build.
        /// @param [out] isOk: If provided, contains the result of the operation (success or failure).
        /// @warning This constructor does not necessarily build the object defined by the parameters.
        /// @warning If you are not sure of the parameters, call default constructor and then Plane::set(const Vector&, const Point&) instead.
        Plane(const Vector& p_normal, const Point& p_point, bool* isOk = nullptr);

        /// @brief Constructor using 3 Points.
        /// @details If 2 of the points coincide or the 3 points are collinear, Plane defaults to YOZ() and @a isOk is set to false (if provided).
        /// @param [in] p_point1: the first point.
        /// @param [in] p_point2: the second point.
        /// @param [in] p_point3: the third point.
        /// @param [out] isOk: If provided, contains the result of the operation (success or failure).
        /// @warning This constructor does not necessarily build the object defined by the parameters.
        Plane(const Point& p_point1, const Point& p_point2, const Point& p_point3, bool* isOk = nullptr);

        // MUTATORS

        /// @brief Updates the Plane components using 4 doubles.
        /// @details If @a p_a, @a p_b and @a p_c equals 0, @a this is left unchanged and method will return false.\n
        /// Otherwise, coordinates will be normalized by the length of normal vector (@a p_a, @a p_b, @a p_c)
        /// and @a this will be defined by p_a*x/k + p_b*y/k +p_c*z/k + p_d/k = 0
        /// with k being the normal vector length, and method will return true.
        /// @return True on success, false otherwise.
        bool set(double p_a, double p_b, double p_c, double p_d);

        /// @brief Updates the Plane components using 3 @link Point Points @endlink.
        /// @details If the @link Point Points @endlink are aligned, @a this is left unchanged and method will return false.\n
        /// Otherwise, @a this will be the plane that contains @a p_point1, @a p_point2 and @a p_point3 \n
        /// Its normal will be (p_point2-p_point1)^(p_point3 - p_point1).\n
        /// @return True on success, false otherwise.
        bool set(const Point& p_point1,const Point& p_point2,const Point& p_point3);

        /// @brief Updates the Plane components using a Vector and a Point.
        /// @details If @a p_normal is null,  @a this is left unchanged and method will return false.\n
        /// Otherwise, @a this will be the plane that contains @a p_point and whose normal is @a p_normal (normalized).\n
        /// @return True on success, false otherwise.
        /// @note The Point provided as parameter does not necessarily define the origin of the Plane.
        bool set(const Vector& p_normal, const Point& p_point);

        // ACCESSORS

        /// @brief Gets the x component of the Plane.
        double a() const;

        /// @brief Gets the y component of the Plane.
        double b() const;

        /// @brief Gets the z component of the Plane.
        double c() const;

        /// @brief Gets the constant component of the Plane.
        double d() const;

        /// @brief Gets the normal vector [a,b,c] of the Plane.
        /// @details It is always a unitary vector.
        Vector normal() const;

        /// @brief Gets the origin of the Plane.
        /// @note The origin of the Plane is defined as the closest Point of the Plane from Point::ORIGIN()
        Point origin() const;

        /// @brief Gets an arbitrary base of the Plane
        /// @param [out] p_i: First Vector of the base. By design, it corresponds to the Vector with the highest @link Vector::length() length @endlink among the projections of Vector::I(), Vector::J() and Vector::K() on this Plane.
        /// @param [out] p_j: Second Vector of the base. By design, it corresponds to p_i^normal()
        /// @note p_i and p_j are unitary
        void getBase(Vector& p_i, Vector& p_j) const;

        /// @brief Perpendicular projection of a Point onto this Plane.
        /// @param [in] p_point: Point to project
        /// @return Projection of @a p_point onto this Plane
        /// @note This method was tested with a length precision of 1E-9 mm.
        Point project(const Point& p_point) const;

        /// @brief Perpendicular projection of a Vector onto this Plane.
        /// @param [in] p_vector: Vector to project
        /// @return Projection of @a p_vector onto this Plane
        /// @note If @a p_vector is normal to this plane, Vector::ZERO() is returned
        Vector project(const Vector& p_vector) const;

        /// @brief Switches the Plane direction.
        /// @details The Plane will define the same region of space, but its @link normal() normal @endlink will be oriented in the opposite direction.
        /// @return A reference to this Plane
        Plane& flip();

        // OPERATORS

        /// @brief Plane strict equality.
        /// @details This method checks that normalized coefficients of each @link Plane Planes @endlink are strictly equals.
        /// @return True if strictly equal
        /// @note This function should not be used to test geometric equality, prefer to use equals() function instead.
        bool operator==(const Plane& p_other) const;

        /// @brief Inequality operator
        /// @return True if @link Plane Planes @endlink are different
        /// @sa operator==()
        bool operator!=(const Plane& p_other) const;

        /// @brief Affectation operator
        /// @details Update the attribute of this Plane with the attribute of @a p_other.
        /// @return A reference to this Plane after being updated.
        Plane& operator=(const Plane& p_other);

        /// @brief Gets the Plane oriented in the opposite direction.
        /// @details This method builds a Plane defining the same region of space but oriented in the opposite direction.
        /// @return Plane in opposite direction.
        /// @sa flip()
        Plane operator-() const;

    protected:
        /// @link Vec4 Vector @endlink (a,b,c,d) defining the plane by the equation ax + by + cz + d = 0
        Vec4 m_plane;
    };

    /// @}
}

/// @cond PRIVATE
#include "Geom/Plane.inl.cpp"
/// @endcond
