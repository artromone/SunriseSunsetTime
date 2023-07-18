#include "SunriseSunsetTime.h"
#include <functional>
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
  DateTime binarySearch(const DateTime &currTime, const std::function< bool(DateTime) > &exitCondition)
  {
    int year = currTime.Year();
    int month = currTime.Month();
    int day = currTime.Day();
    DateTime dayStart(year, month, day, 0, 0, 0, 0);
    DateTime dayEnd(year, month, day, 23, 59, 59, 999999);
    DateTime lowerBound = dayStart;
    DateTime upperBound = dayEnd;
    DateTime result = dayStart;
    while (lowerBound <= upperBound)
    {
      DateTime mid = DateTime(lowerBound.Ticks() + (upperBound.Ticks() - lowerBound.Ticks()) / 2);
      if (exitCondition(mid))
      {
        result = mid;
        upperBound = mid - TimeSpan(1);
      }
      else
      {
        lowerBound = mid + TimeSpan(1);
      }
    }
    return result;
  }
}
std::pair< DateTime, DateTime > getSunriseAndSunsetTime(const DateTime &currTime, const Observer &observer)
{
  DateTime sunrise = binarySearch(currTime, [&observer](DateTime dt)
  {
    double midSunElevation = getSunElevation(dt, observer);
    return midSunElevation > sunCriticalAngle;
  });
  DateTime sunset = binarySearch(currTime, [&observer](DateTime dt)
  {
    double midSunElevation = getSunElevation(dt, observer);
    return midSunElevation <= sunCriticalAngle;
  });
  return std::make_pair(sunrise, sunset);
}
