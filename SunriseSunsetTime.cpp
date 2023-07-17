#include "SunriseSunsetTime.h"
#include "SGP4/CoordTopocentric.h"
#include <SGP4/SolarPosition.h>
namespace SGP4
{
  namespace
  {
    double getSunElevation(const DateTime &currTime, Observer observer)
    {
      SolarPosition solarPosition;
      Eci sunEci = solarPosition.FindPosition(currTime);
      CoordTopocentric lookAngle = observer.GetLookAngle(sunEci);
      double sunElevation = SGP4::Util::RadiansToDegrees(lookAngle.elevation);
      return sunElevation;
    }
  }
  std::pair< DateTime, DateTime > getSunriseAndSunsetTime(const DateTime &currTime, Observer observer)
  {
  }
}
