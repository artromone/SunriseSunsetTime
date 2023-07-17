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


#ifndef ECI_H_
#define ECI_H_

#include "CoordGeodetic.h"
#include "Vector.h"
#include "DateTime.h"

namespace SGP4 {

/**
 * @brief Stores an Earth-centered inertial position for a particular time.
 */
class SGP4_DECL Eci
{
public:
    Eci()
        : m_dt( 0 ), m_position( 0.0, 0.0, 0.0 ), m_velocity( 0.0, 0.0, 0.0 )
    {
        
    }
    Eci( Eci const& rh )
        : m_dt( rh.m_dt )
        , m_position( rh.m_position )
        , m_velocity( rh.m_velocity )
    {

    }
    Eci &operator=( Eci const& ) = default;

    /**
     * @param[in] dt the date to be used for this position
     * @param[in] latitude the latitude in degrees
     * @param[in] longitude the longitude in degrees
     * @param[in] altitude the altitude in kilometers
     */
    Eci( const DateTime& dt,
         const double latitude,
         const double longitude,
         const double altitude )
    {
        ToEci( dt, CoordGeodetic( latitude, longitude, altitude ) );
    }

    /**
     * @param[in] dt the date to be used for this position
     * @param[in] geo the position
     */
    Eci( const DateTime& dt, const CoordGeodetic& geo )
    {
        ToEci( dt, geo );
    }

    /**
     * @param[in] dt the date to be used for this position
     * @param[in] position the position
     */
    Eci( const DateTime &dt, const Vector &position )
        : m_dt( dt )
        , m_position( position )
    {
    }

    /**
     * @param[in] dt the date to be used for this position
     * @param[in] position the position
     * @param[in] velocity the velocity
     */
    Eci( const DateTime &dt, const Vector &position, const Vector &velocity )
        : m_dt( dt )
        , m_position( position )
        , m_velocity( velocity )
    {
    }

    /**
     * Equality operator
     * @param dt the date to compare
     * @returns true if the object matches
     */
    bool operator==( const DateTime& dt ) const
    {
        return m_dt == dt;
    }

    /**
     * Inequality operator
     * @param dt the date to compare
     * @returns true if the object doesn't match
     */
    bool operator!=( const DateTime& dt ) const
    {
        return m_dt != dt;
    }

    /**
     * Update this object with a new date and geodetic position
     * @param dt new date
     * @param geo new geodetic position
     */
    void Update( const DateTime& dt, const CoordGeodetic& geo )
    {
        ToEci( dt, geo );
    }

    /**
     * @returns the position
     */
    Vector Position() const
    {
        return m_position;
    }

    /**
     * @returns the velocity
     */
    Vector Velocity() const
    {
        return m_velocity;
    }

    /**
     * @returns the date
     */
    DateTime GetDateTime() const
    {
        return m_dt;
    }

    /**
     * @returns the position in geodetic form
     */
    CoordGeodetic ToGeodetic() const;

private:
    void ToEci( const DateTime& dt, const CoordGeodetic& geo );

    DateTime m_dt;
    Vector m_position;
    Vector m_velocity;
};

struct EciDiff
{
    TimeSpan d_time;
    Vector d_position;
    Vector d_velocity;
};

SGP4_DECL bool operator==( Eci const& lh, Eci const& rh );
SGP4_DECL Eci operator+( Eci const& lh, EciDiff const& rh );
SGP4_DECL Eci operator+( EciDiff const& rh, Eci const& lh );
SGP4_DECL EciDiff operator-( Eci const& lh, Eci const& rh );
SGP4_DECL EciDiff operator*( EciDiff const& lh, double const mul );
SGP4_DECL EciDiff operator*( double const mul, EciDiff const& lh );
SGP4_DECL EciDiff operator/( EciDiff const& lh, double const div );

} //namespace SGP4

using SGP4::operator+;
using SGP4::operator-;
using SGP4::operator*;
using SGP4::operator/;

#endif
