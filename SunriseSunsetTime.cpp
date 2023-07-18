#include "SunriseSunsetTime.h"
#include <SGP4/CoordTopocentric.h>
#include <SGP4/SolarPosition.h>
using namespace SGP4;
namespace
{
  int sunCriticalAngle = 0;
  double getSunElevation(const DateTime &currTime, Observer observer)
  {
    Eci sunEci = SolarPosition().FindPosition(currTime);
    CoordTopocentric lookAngle = observer.GetLookAngle(sunEci);
    return Util::RadiansToDegrees(lookAngle.elevation);
  }
  DateTime binarySearchSunrise(const DateTime &start, const DateTime &end, const Observer &observer)
  {
    DateTime left = start;
    DateTime right = end;
    DateTime result = start;
    while (left <= right)
    {
      DateTime mid = DateTime(left.Ticks() + (right.Ticks() - left.Ticks()) / 2);
      double midSunElevation = getSunElevation(mid, observer);
      if (midSunElevation > sunCriticalAngle)
      {
        result = mid;
        right = mid - TimeSpan(1);
      }
      else
      {
        left = mid + TimeSpan(1);
      }
    }
    return result;
  }
  DateTime binarySearchSunset(const DateTime &start, const DateTime &end, const Observer &observer)
  {
    DateTime left = start;
    DateTime right = end;
    DateTime result = end;
    while (left <= right)
    {
      DateTime mid = DateTime(left.Ticks() + (right.Ticks() - left.Ticks()) / 2);
      double midSunElevation = getSunElevation(mid, observer);
      if (midSunElevation <= sunCriticalAngle)
      {
        result = mid;
        right = mid - TimeSpan(1);
      }
      else
      {
        left = mid + TimeSpan(1);
      }
    }
    return result;
  }
}
std::pair< DateTime, DateTime > getSunriseAndSunsetTime(const DateTime &currTime, const Observer &observer)
{
  int year = currTime.Year();
  int month = currTime.Month();
  int day = currTime.Day();
  DateTime dayStart(year, month, day, 0, 0, 0, 0);
  DateTime dayEnd(year, month, day, 23, 59, 59, 999999);
  DateTime sunrise = binarySearchSunrise(dayStart, dayEnd, observer);
  DateTime sunset = binarySearchSunset(dayStart, dayEnd, observer);
  return std::make_pair(sunrise, sunset);
}
