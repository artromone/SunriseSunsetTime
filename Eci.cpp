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


#include <SGP4/Eci.h>
#include <SGP4/Globals.h>
#include <SGP4/Util.h>

namespace SGP4 {

/**
 * Converts a DateTime and Geodetic position to Eci coordinates
 * @param[in] dt the date
 * @param[in] geo the geodetic position
 */
void Eci::ToEci( const DateTime& dt, const CoordGeodetic &geo )
{
    /*
     * set date
     */
    m_dt = dt;

    static const double mfactor = kTWOPI * ( kOMEGA_E / kSECONDS_PER_DAY );

    /*
     * Calculate Local Mean Sidereal Time for observers longitude
     */
    const double theta = m_dt.ToLocalMeanSiderealTime( geo.longitude );

    /*
     * take into account earth flattening
     */
    const double c = 1.0
        / sqrt( 1.0 + kF * ( kF - 2.0 ) * pow( sin( geo.latitude ), 2.0 ) );
    const double s = pow( 1.0 - kF, 2.0 ) * c;
    const double achcp = ( kXKMPER * c + geo.altitude ) * cos( geo.latitude );

    /*
     * X position in km
     * Y position in km
     * Z position in km
     * W magnitude in km
     */
    m_position.x = achcp * cos( theta );
    m_position.y = achcp * sin( theta );
    m_position.z = ( kXKMPER * s + geo.altitude ) * sin( geo.latitude );
    m_position.w = m_position.Magnitude();

    /*
     * X velocity in km/s
     * Y velocity in km/s
     * Z velocity in km/s
     * W magnitude in km/s
     */
    m_velocity.x = -mfactor * m_position.y;
    m_velocity.y = mfactor * m_position.x;
    m_velocity.z = 0.0;
    m_velocity.w = m_velocity.Magnitude();
}

/**
 * @returns the position in geodetic form
 */
CoordGeodetic Eci::ToGeodetic() const
{
    const double theta = Util::AcTan( m_position.y, m_position.x );

    const double lon = Util::WrapNegPosPI( theta
                                           - m_dt.ToGreenwichSiderealTime() );

    const double r = sqrt( ( m_position.x * m_position.x )
                           + ( m_position.y * m_position.y ) );

    static const double e2 = kF * ( 2.0 - kF );

    double lat = Util::AcTan( m_position.z, r );
    double phi = 0.0;
    double c = 0.0;
    int cnt = 0;

    do
    {
        phi = lat;
        const double sinphi = sin( phi );
        c = 1.0 / sqrt( 1.0 - e2 * sinphi * sinphi );
        lat = Util::AcTan( m_position.z + kXKMPER * c * e2 * sinphi, r );
        cnt++;
    } while ( fabs( lat - phi ) >= 1e-10 && cnt < 10 );

    const double alt = r / cos( lat ) - kXKMPER * c;

    return CoordGeodetic( lat, lon, alt, true );
}

SGP4_DECL bool operator==( Eci const& lh, Eci const& rh )
{
    return lh.Position() == rh.Position() && lh.GetDateTime() == rh.GetDateTime();
}

SGP4_DECL Eci operator+( Eci const& eci, EciDiff const& diff )
{
    return { DateTime( eci.GetDateTime().Ticks() + diff.d_time.Ticks() ),
        eci.Position() + diff.d_position,
        eci.Velocity() + diff.d_velocity };
}

SGP4_DECL Eci operator+( EciDiff const& diff, Eci const& eci )
{
    return { DateTime( eci.GetDateTime().Ticks() + diff.d_time.Ticks() ),
        eci.Position() + diff.d_position,
        eci.Velocity() + diff.d_velocity };
}

SGP4_DECL EciDiff operator-( Eci const& lh, Eci const& rh )
{
    return { TimeSpan{ lh.GetDateTime().Ticks() - rh.GetDateTime().Ticks() },
        lh.Position() - rh.Position(),
        lh.Velocity() - rh.Velocity() };
}

SGP4_DECL EciDiff operator*( EciDiff const& diff, double const mul )
{
    return { TimeSpan{ int64_t( diff.d_time.Ticks() * mul ) },
        diff.d_position * mul,
        diff.d_velocity * mul };
}

SGP4_DECL EciDiff operator*( double const mul, EciDiff const& diff )
{
    return { TimeSpan{ int64_t( diff.d_time.Ticks() * mul ) },
        diff.d_position * mul,
        diff.d_velocity * mul };
}

SGP4_DECL EciDiff operator/( EciDiff const& diff, double const div )
{
    return { TimeSpan{ int64_t( diff.d_time.Ticks() / div ) },
        diff.d_position / div,
        diff.d_velocity / div };
}

} //namespace SGP4
