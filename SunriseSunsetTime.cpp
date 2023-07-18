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
    void determineHours(const Observer &observer, DateTime &dt_sunrise, DateTime &dt_sunset)
    {
      int year = dt_sunrise.Year();
      int month = dt_sunrise.Month();
      int day = dt_sunrise.Day();
      int hour_sunrise = -1;
      int hour_sunset = -1;
      for (int hour = 0; hour < 24; ++hour)
      {
        DateTime dt(year, month, day, hour,
                    0, 0, 0);
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
      dt_sunrise = DateTime(year, month, day,
                            hour_sunrise, 0, 0, 0);
      dt_sunset = DateTime(year, month, day,
                           hour_sunset, 0, 0, 0);
    }
    void determineMinutes(const Observer &observer, DateTime &dt_sunrise, DateTime &dt_sunset)
    {
      int year = dt_sunrise.Year();
      int month = dt_sunrise.Month();
      int day = dt_sunrise.Day();
      int hour_sunrise = dt_sunrise.Hour();
      int hour_sunset = dt_sunset.Hour();
      int minute_sunrise = -1;
      int minute_sunset = -1;
      for (int minute = 0; minute < 60; ++minute)
      {
        DateTime dt_sunrise_(year, month, day,
                             hour_sunrise, minute, 0, 0);
        DateTime dt_sunset_(year, month, day,
                            hour_sunset, minute, 0, 0);
        double sunElevRise = getSunElevation(dt_sunrise_, observer);
        double sunElevSet = getSunElevation(dt_sunset_, observer);
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
      dt_sunrise = DateTime(year, month, day,
                            hour_sunrise, minute_sunrise, 0, 0);
      dt_sunset = DateTime(year, month, day,
                           hour_sunset, minute_sunset, 0, 0);
    }
    void determineSeconds(const Observer &observer, DateTime &dt_sunrise, DateTime &dt_sunset)
    {
      int year = dt_sunrise.Year();
      int month = dt_sunrise.Month();
      int day = dt_sunrise.Day();
      int hour_sunrise = dt_sunrise.Hour();
      int hour_sunset = dt_sunset.Hour();
      int minute_sunrise = dt_sunrise.Minute();
      int minute_sunset = dt_sunset.Minute();
      int second_sunrise = -1;
      int second_sunset = -1;
      for (int second = 0; second < 60; ++second)
      {
        DateTime dt_sunrise_(year, month, day,
                             hour_sunrise, minute_sunrise, second, 0);
        DateTime dt_sunset_(year, month, day,
                            hour_sunset, minute_sunset, second, 0);
        double sunElevRise = getSunElevation(dt_sunrise_, observer);
        double sunElevSet = getSunElevation(dt_sunset_, observer);
        if (sunElevRise > 0 && second_sunrise == -1)
        {
          second_sunrise = second - 1;
        }
        else if (sunElevSet <= 0 && second_sunrise != -1)
        {
          second_sunset = second;
          break;
        }
      }
      dt_sunrise = DateTime(year, month, day,
                            hour_sunrise, minute_sunrise, second_sunrise, 0);
      dt_sunset = DateTime(year, month, day,
                           hour_sunset, minute_sunset, second_sunset, 0);
    }
    void determineMillisecond(const Observer &observer, DateTime &dt_sunrise, DateTime &dt_sunset)
    {
      int year = dt_sunrise.Year();
      int month = dt_sunrise.Month();
      int day = dt_sunrise.Day();
      int hour_sunrise = dt_sunrise.Hour();
      int hour_sunset = dt_sunset.Hour();
      int minute_sunrise = dt_sunrise.Minute();
      int minute_sunset = dt_sunset.Minute();
      int second_sunrise = dt_sunrise.Second();
      int second_sunset = dt_sunset.Second();
      int millisecond_sunrise = -1;
      int millisecond_sunset = -1;
      for (int millisecond = 0; millisecond < 1000000; millisecond += 1000)
      {
        DateTime dt_sunrise_(year, month, day, hour_sunrise,
                             minute_sunrise, second_sunrise, millisecond);
        DateTime dt_sunset_(year, month, day, hour_sunset,
                            minute_sunset, second_sunset, millisecond);
        double sunElevRise = getSunElevation(dt_sunrise_, observer);
        double sunElevSet = getSunElevation(dt_sunset_, observer);
        if (sunElevRise > 0 && millisecond_sunrise == -1)
        {
          millisecond_sunrise = millisecond - 1000;
        }
        else if (sunElevSet <= 0 && millisecond_sunrise != -1)
        {
          millisecond_sunset = millisecond;
          break;
        }
      }
      dt_sunrise = DateTime(year, month, day, hour_sunrise,
                            minute_sunrise, second_sunrise, millisecond_sunrise);
      dt_sunset = DateTime(year, month, day, hour_sunset,
                           minute_sunset, second_sunset, millisecond_sunset);
    }
  }
  std::pair< DateTime, DateTime > getSunriseAndSunsetTime(const DateTime &currTime, const Observer &observer)
  {
    int year = currTime.Year();
    int month = currTime.Month();
    int day = currTime.Day();
    DateTime dt_sunrise(year, month, day, 0, 0, 0, 0);
    DateTime dt_sunset(year, month, day, 0, 0, 0, 0);
    determineHours(observer, dt_sunrise, dt_sunset);
    determineMinutes(observer, dt_sunrise, dt_sunset);
    determineSeconds(observer, dt_sunrise, dt_sunset);
    determineMillisecond(observer, dt_sunrise, dt_sunset);
    return std::make_pair(dt_sunrise, dt_sunset);
  }
}
