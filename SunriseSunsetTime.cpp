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
  std::pair< DateTime, DateTime > getSunriseAndSunsetTime(const DateTime &currTime, const Observer &observer)
  {
    int year = currTime.Year();
    int month = currTime.Month();
    int day = currTime.Day();

    int hour_sunrise = -1;
    int hour_sunset = -1;

    for (int hour = 0; hour < 24; ++hour)
    {
      DateTime dt(year, month, day, hour, 0, 0, 0);
      double sunElevation = getSunElevation(dt, observer);
      if (sunElevation > 0 && hour_sunrise == -1)
      {
        hour_sunrise = hour - 1;
      }
      else if (sunElevation <= 0 && hour_sunrise != -1)
      {
        hour_sunset = hour;
        break;
      }
    }
    if (hour_sunrise == -1)
    {
      throw std::runtime_error("Sunrise does not occur on this date at this location.");
    }

    int minute_sunrise = -1;
    int minute_sunset = -1;

    for (int minute = 0; minute < 60; ++minute)
    {
      DateTime dt_sunrise(year, month, day, hour_sunrise, minute, 0, 0);
      DateTime dt_sunset(year, month, day, hour_sunset, minute, 0, 0);
      double sunElevRise = getSunElevation(dt_sunrise, observer);
      double sunElevSet = getSunElevation(dt_sunset, observer);
      if (sunElevRise > 0 && minute_sunrise == -1)
      {
        minute_sunrise = minute - 1;
      }
      else if (sunElevSet <= 0 && minute_sunrise != -1)
      {
        minute_sunset = minute;
        break;
      }
    }

    DateTime dt_sunrise(year, month, day,
                        hour_sunrise, minute_sunrise, 0, 0);
    DateTime dt_sunset(year, month, day,
                       hour_sunset, minute_sunset, 0, 0);

    return std::make_pair(dt_sunrise, dt_sunset);
  }
}
