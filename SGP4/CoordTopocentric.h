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


#ifndef COORDTOPOCENTRIC_H_
#define COORDTOPOCENTRIC_H_

#include "Util.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <tuple>

namespace SGP4 {

/**
 * @brief Stores a topocentric location (azimuth, elevation, range and range
 * rate).
 *
 * Azimuth and elevation are stored in radians. Range in kilometres. Range
 * rate in kilometres/second.
 */
struct CoordTopocentric
{
    static constexpr double __pi = 3.14159265358979323846;
    static constexpr double __pi_2 = 3.14159265358979323846 * 2;
public:
    /**
     * Default constructor
     */
    CoordTopocentric()
        : azimuth( 0.0 )
        , elevation( 0.0 )
        , range( 0.0 )
        , range_rate( 0.0 )
    {
    }

    /**
     * Constructor
     * @param[in] az azimuth in radians
     * @param[in] el elevation in radians
     * @param[in] rnge range in kilometers
     * @param[in] rnge_rate range rate in kilometers per second
     */
    CoordTopocentric(
        double az,
        double el,
        double rnge,
        double rnge_rate )
        : azimuth( az )
        , elevation( el )
        , range( rnge )
        , range_rate( rnge_rate )
    {
    }

    /**
     * Copy constructor
     * @param[in] topo object to copy from
     */
    CoordTopocentric( const CoordTopocentric& topo )
    {
        azimuth = topo.azimuth;
        elevation = topo.elevation;
        range = topo.range;
        range_rate = topo.range_rate;
    }

    /**
     * Assignment operator
     * @param[in] topo object to copy from
     */
    CoordTopocentric& operator=( const CoordTopocentric& topo )
    {
        if ( this != &topo )
        {
            azimuth = topo.azimuth;
            elevation = topo.elevation;
            range = topo.range;
            range_rate = topo.range_rate;
        }
        return *this;
    }

    /**
     * Dump this object to a string
     * @returns string
     */
    std::string ToString() const
    {
        std::stringstream ss;
        ss << std::right << std::fixed << std::setprecision( 3 );
        ss << "Az: " << std::setw( 8 ) << Util::RadiansToDegrees( azimuth );
        ss << ", El: " << std::setw( 8 ) << Util::RadiansToDegrees( elevation );
        ss << ", Rng: " << std::setw( 10 ) << range;
        ss << ", Rng Rt: " << std::setw( 7 ) << range_rate;
        return ss.str();
    }

    /** azimuth in radians */
    double azimuth;
    /** elevations in radians */
    double elevation;
    /** range in kilometers */
    double range;
    /** range rate in kilometers per second */
    double range_rate;

    void normalize()
    {
        while ( azimuth >= __pi_2 )
        {
            azimuth -= __pi_2;
        }
        while ( azimuth < 0 )
        {
            azimuth += __pi_2;
        }
    }
    CoordTopocentric normalized() const
    {
        CoordTopocentric copy( *this );
        copy.normalize();
        return copy;
    }
};

struct CoordTopocentricDiff
{
    double d_azimuth;
    double d_elevation;
    double d_range;
    double d_range_rate;

    void normalize()
    {
        while ( d_azimuth > CoordTopocentric::__pi )
        {
            d_azimuth -= CoordTopocentric::__pi_2;
        }
        while ( d_azimuth < -CoordTopocentric::__pi )
        {
            d_azimuth += CoordTopocentric::__pi_2;
        }
    }
    CoordTopocentricDiff normalized() const
    {
        CoordTopocentricDiff result( *this );
        result.normalize();
        return result;
    }
};

inline bool operator==( CoordTopocentric  const& lh, CoordTopocentric const& rh)
{
    return std::tie( lh.azimuth, lh.elevation, lh.range, lh.range_rate )
        == std::tie( rh.azimuth, rh.elevation, rh.range, rh.range_rate );
}

inline CoordTopocentric operator+( CoordTopocentric const& coords, CoordTopocentricDiff const& diff )
{
    return { coords.azimuth + diff.d_azimuth,
             coords.elevation + diff.d_elevation,
             coords.range + diff.d_range,
             coords.range_rate + diff.d_range_rate };
}

inline CoordTopocentric operator+( CoordTopocentricDiff const& diff, CoordTopocentric const& coords )
{
    return operator+( coords, diff );
}


inline CoordTopocentricDiff operator-( CoordTopocentric const& lh, CoordTopocentric const& rh )
{
    return CoordTopocentricDiff{ lh.azimuth - rh.azimuth,
        lh.elevation - rh.elevation,
        lh.range - rh.range,
        lh.range_rate - rh.range_rate };
    
}

inline CoordTopocentricDiff operator*( CoordTopocentricDiff const& diff, double const mul )
{
    return CoordTopocentricDiff{ diff.d_azimuth * mul,
             diff.d_elevation * mul,
             diff.d_range * mul,
             diff.d_range_rate * mul };
}

inline CoordTopocentricDiff operator*( double const mul, CoordTopocentricDiff const& diff )
{
    return operator*( diff, mul );
}

inline CoordTopocentricDiff operator/( CoordTopocentricDiff const& diff, double const div )
{
    return { diff.d_azimuth / div,
             diff.d_elevation / div,
             diff.d_range / div,
             diff.d_range_rate / div };
}

} //namespace SGP4

inline std::ostream& operator<<(std::ostream& strm, const SGP4::CoordTopocentric& t)
{
    return strm << t.ToString();
}

using SGP4::operator+;
using SGP4::operator-;
using SGP4::operator*;
using SGP4::operator/;

#endif
