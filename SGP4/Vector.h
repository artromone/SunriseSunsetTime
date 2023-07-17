/*
 * Copyright 2013 Daniel Warner <contact@danrw.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef VECTOR_H_
#define VECTOR_H_

#include "Decl.h"

#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

namespace SGP4 {

/**
 * @brief Generic vector
 *
 * Stores x, y, z, w
 */
struct SGP4_DECL Vector
{
public:

    /**
     * Default constructor
     */
    Vector()
        : x( 0.0 ), y( 0.0 ), z( 0.0 ), w( 0.0 )
    {
    }

    /**
     * Constructor
     * @param arg_x x value
     * @param arg_y y value
     * @param arg_z z value
     */
    Vector( const double arg_x,
            const double arg_y,
            const double arg_z )
        : x( arg_x ), y( arg_y ), z( arg_z ), w( 0.0 )
    {
    }

    /**
     * Constructor
     * @param arg_x x value
     * @param arg_y y value
     * @param arg_z z value
     * @param arg_w w value
     */
    Vector( const double arg_x,
            const double arg_y,
            const double arg_z,
            const double arg_w )
        : x( arg_x ), y( arg_y ), z( arg_z ), w( arg_w )
    {
    }

    /**
     * Copy constructor
     * @param v value to copy from
     */
    Vector( const Vector& v )
    {
        x = v.x;
        y = v.y;
        z = v.z;
        w = v.w;
    }

    /**
     * Assignment operator
     * @param v value to copy from
     */
    Vector& operator=( const Vector& v )
    {
        if ( this != &v )
        {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
        }
        return *this;
    }

    /**
     * Subtract operator
     * @param v value to suctract from
     */
    Vector operator-( const Vector& v ) const
    {
        return Vector( x - v.x,
                       y - v.y,
                       z - v.z,
                       0.0 );
    }

    Vector operator+( const Vector& v ) const
    {
        return Vector( x + v.x,
                       y + v.y,
                       z + v.z,
                       0.0 );
    }

    Vector operator*( double mul ) const
    {
        return Vector( x * mul, y * mul, z * mul );
    }

    Vector operator/( double div ) const
    {
        return Vector( x / div, y / div, z / div );
    }

    bool operator==( Vector const& rh ) const
    {
        return std::tie( x, y, z, w ) == std::tie( rh.x, rh.y, rh.z, rh.w );
    }

    /**
     * Calculates the magnitude of the vector
     * @returns magnitude of the vector
     */
    double Magnitude() const
    {
        return sqrt( x * x + y * y + z * z );
    }

    /**
     * Calculates the dot product
     * @returns dot product
     */
    double Dot( const Vector& vec ) const
    {
        return ( x * vec.x ) +
            ( y * vec.y ) +
            ( z * vec.z );
    }

    /**
     * Converts this vector to a string
     * @returns this vector as a string
     */
    std::string ToString() const
    {
        std::stringstream ss;
        ss << std::right << std::fixed << std::setprecision( 3 );
        ss << "X: " << std::setw( 9 ) << x;
        ss << ", Y: " << std::setw( 9 ) << y;
        ss << ", Z: " << std::setw( 9 ) << z;
        ss << ", W: " << std::setw( 9 ) << w;
        return ss.str();
    }

    /** x value */
    double x;
    /** y value */
    double y;
    /** z value */
    double z;
    /** w value */
    double w;
};

} //namespace SGP4

inline std::ostream& operator<<(std::ostream& strm, const SGP4::Vector& v)
{
    return strm << v.ToString();
}

#endif
