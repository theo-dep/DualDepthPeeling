#pragma once

/// @file Vec4.h
/// @brief Contains the declaration of class gps::geom::Vec4
/// @ingroup geom

#include <QtCore/QString>

namespace geom
{

    /// @addtogroup geom
    /// @{

    /// @class Vec4 Vec4.h "GPS/Geom/Vec4.h"
    /// @brief Class describing a vector of 4 doubles.
    /// @details This class is used to manipulate 4-doubles vectors.\n
    /// It provides basic operators :
    /// @link Vec4::operator+() addition @endlink,
    /// @link Vec4::operator-(const Vec4&) const subtraction @endlink,
    /// @link Vec4::operator*() dot product @endlink,
    /// @link Vec4::operator==() equality @endlink,
    /// @link Vec4::operator!=() inequality @endlink,
    /// @link Vec4::operator=() affectation @endlink and
    /// @link Vec4::operator-() const opposite @endlink \n
    /// It also provides norm computations of three different kinds:
    /// @link oneNorm() one-Norm @endlink,
    /// @link twoNorm() two-Norm @endlink and
    /// @link infNorm() infinite Norm @endlink.\n
    /// @note The formulas used in the implementation of this class were obtained from the paper "Plucker Coordinates for Lines in the Space" written by Yan-Bin Jia.
    /// @note This class was not built to represent a specific geometric object.
    /// @note Its purpose is to facilitate computations involving 4-doubles vectors, regardless of what they represent.
    /// @note No cross-product is defined because it does not exist for 4D vectors.

    class Vec4
    {
        // This method is not a member of Vec4 because the common use is to write factor*vec.
        /// @brief Multiplies a @link Vec4 Vector @endlink by a scalar.
        /// @details This method multiply each coefficient of the provided @link Vec4 Vector @endlink by the provided scalar.
        /// @param [in] factor: Scalar to use for multiplication.
        /// @param [in] vec: @link Vec4 Vector @endlink to multiply.
        /// @return A @link Vec4 Vector @endlink containing the result of the multiplication.
        friend Vec4 operator*(double factor, const Vec4& vec);

    public:

        /// @brief Constant @link Vec4 Vector @endlink with all elements set to 0
        static const Vec4& ZERO();

        // CONSTRUCTORS

        /// @brief Default constructor.
        /// @details This initializes the @link Vec4 Vector @endlink to Vec4::ZERO()
        Vec4();

        /// @brief Copy constructor
        /// @details Builds a @link Vec4 Vector @endlink using the @link Vec4 Vector @endlink provided as parameter.
        Vec4(const Vec4& other);

        /// @brief Constructor using 4 doubles.
        /// @details Build a @link Vec4 Vector @endlink using the 4 doubles provided as parameters.
        /// @param [in] a: First element
        /// @param [in] b: Second element
        /// @param [in] c: Third element
        /// @param [in] d: Fourth element
        explicit Vec4(double a, double b, double c, double d);

        /// @brief Constructor using an array of 4 doubles.
        /// @details Build a @link Vec4 Vector @endlink using the array provided as parameter.
        /// If @a p_array is null, @link Vec4 Vector @endlink defaults to ZERO() and @a isOk is set to false (if provided).
        /// @param [in] p_array: Array containing the elements (p_array[0] is the first element, p_array[1] the second,...). If @a p_array is null, default constructor is used and isOk is set to false.
        /// @param [out] isOk: If provided, contains the result of the operation (success or failure).
        /// @warning This constructor does not necessarily build the object defined by the parameters.
        /// @warning If you are not sure of the parameters, call default constructor and then Vec4::set(const double[4], bool*) instead.
        explicit Vec4(const double p_array[4], bool* isOk = nullptr);

        // MUTATORS

        /// @brief Updates the @link Vec4 Vector @endlink elements using 4 doubles.
        /// @param [in] a: First element
        /// @param [in] b: Second element
        /// @param [in] c: Third element
        /// @param [in] d: Fourth element
        void set(double a, double b, double c, double d);

        /// @brief Updates the @link Vec4 Vector @endlink elements using an array of 4 doubles.
        /// @details If @a p_array is null, @link Vec4 Vector @endlink is unchanged and @a isOk is set to false (if provided).
        /// @param [in] p_array: Array containing the elements (p_array[0] is the first element, p_array[1] the second,...)
        /// @return The status of the operation (success or failure).
        /// @warning It is assumed that @a p_array defines an array of 4 doubles. If @a p_array is null, @a this is unchanged and isOk is set to false.
        bool set(const double p_array[4]);

        /// @brief Updates the @link Vec4 Vector @endlink by settings all of its elements to value @a value.
        /// @param [in] value: Value to set
        void fill(double value);

        /// @brief Updates a specific element of the @link Vec4 Vector @endlink.
        /// @details Updates the element in index @a idx using the double provided as parameter.\n
        /// If the index is out of bounds, method returns false and @link Vec4 Vector @endlink is left unchanged.
        /// @param [in] idx: Index of the element to update
        /// @param [in] value: New value of the element.
        /// @return True on success, false otherwise.
        bool set(int idx, double value);

        // ACCESSORS

        /// @brief Access elements by index.
        /// @return Reference to the element at index @a idx. Can be used for fast update.
        /// @warning This function does not check if the provided index is in bounds.
        /// If you are not sure that the index is in bound, you should use operator[]() instead.
        /// @sa operator[]()
        double& at(int idx);

        /// @brief Access elements by index.
        /// @details Can be us
        /// @return The element at index @a idx.
        /// @warning This function does not check if the provided index is in bounds.
        /// If you are not sure that the index is in bound, you should use operator[]() instead.
        /// @sa operator[]()
        double at(int idx) const;

        // COMPUTERS

        /// @brief Gets the length of the @link Vec4 Vector @endlink.
        /// @details The length of the @link Vec4 Vector @endlink corresponds to its twoNorm().
        double length() const;

        /// @brief Gets the squared length of the @link Vec4 Vector @endlink.
        /// @details The length of the @link Vec4 Vector @endlink corresponds to its twoNorm().
        double squaredLength() const;

        // OPERATORS

        /// @brief Addition operator
        /// @return @a this + @a other
        Vec4 operator+(const Vec4& other) const;

        /// @brief Subtraction operator
        /// @return @a this - @a other
        Vec4 operator-(const Vec4& other) const;

        /// @brief Opposite operator
        /// @return -1 * @a this
        Vec4 operator-() const;

        /// @brief Dot product operator
        /// @return (@a this[0] * @a other[0]) + (@a this[1] * @a other[1]) + (@a this[2] * @a other[2]) + (@a this[3] * @a other[3])
        double operator*(const Vec4& other) const;

        /// @brief Multiplies by a scalar.
        /// @return @a this * @a factor
        Vec4 operator*(double factor) const;

        /// @brief In-place addition operator
        /// @return A reference to @a this
        Vec4& operator+=(const Vec4& other);

        /// @brief In-place subtraction operator
        /// @return A reference to @a this
        Vec4& operator-=(const Vec4& other);

        /// @brief In-place multiplication by a scalar
        /// @return A reference to @a this
        Vec4& operator*=(double factor);

        /// @brief Equality operator
        /// @details Two @link Vec4 Vectors @endlink are equal if all their elements are equal.
        /// @return True if @a this is equal to @a other
        bool operator==(const Vec4& other) const;

        /// @brief Inequality operator
        /// @return True if @a is different from @a other
        /// @sa operator==()
        bool operator!=(const Vec4& other) const;

        /// @brief Affectation operator
        /// @details Sets the elements of @a this to the elements of @a other
        /// @return A reference to @a this
        Vec4& operator=(const Vec4& other);

        /// @brief Access elements by index.
        /// @return Element at index @a idx if in-bounds, nan("") if out of bounds.
        /// @warning This function checks if the provided index is in bounds.
        /// If you are sure that the index is in bound, you can use at() instead for faster access.
        /// @sa at()
        double operator[](int idx) const;

    private:
        /// @brief Array of doubles containing the elements
        double m_array[4];
    };

    /// @}
}

/// @cond PRIVATE
#include "Geom/Vec4.inl.cpp"
/// @endcond
