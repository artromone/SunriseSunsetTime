#include "SunriseSunsetTime.h"
#include <SGP4/CoordTopocentric.h>
#include <SGP4/SolarPosition.h>
using namespace SGP4;
namespace
{
  int sunCriticalAngle = 0;
  double getSunElevation(const DateTime &currTime, Observer observer)
  {
    SolarPosition solarPosition;
    Eci sunEci = solarPosition.FindPosition(currTime);
    CoordTopocentric lookAngle = observer.GetLookAngle(sunEci);
    double sunElevation = Util::RadiansToDegrees(lookAngle.elevation);
    return sunElevation;
  }
}
std::pair< DateTime, DateTime > getSunriseAndSunsetTime(const DateTime &currTime, const Observer &observer)
{
  int year = currTime.Year();
  int month = currTime.Month();
  int day = currTime.Day();
  DateTime dayStart(year, month, day, 0, 0, 0, 0);
  DateTime dayEnd(year, month, day, 23, 59, 59, 999999);
  int range = 10;
  int64_t ticksLowerBound = dayStart.Ticks();
  int64_t ticksUpperBound = dayEnd.Ticks();
  int64_t step = (ticksUpperBound - ticksLowerBound) / range;
  for (int64_t ticks = ticksLowerBound; step != 0; ticks += step)
  {
    double sunElevation = getSunElevation(DateTime(ticks), observer);
    if (sunElevation > sunCriticalAngle)
    {
      ticksUpperBound = ticks;
      ticksLowerBound = ticks - step;
      step = (ticksUpperBound - ticksLowerBound) / range;
    }
  }
  DateTime sunrise = DateTime(ticksLowerBound);
  ticksLowerBound = sunrise.Ticks();
  ticksUpperBound = dayEnd.Ticks();
  step = (ticksUpperBound - ticksLowerBound) / range;
  double prevSunElevation = sunCriticalAngle - 1;
  for (int64_t ticks = ticksLowerBound; step != 0; ticks += step)
  {
    double sunElevation = getSunElevation(DateTime(ticks), observer);
    if (sunElevation <= sunCriticalAngle && prevSunElevation > sunElevation)
    {
      ticksUpperBound = ticks;
      ticksLowerBound = ticks - step;
      step = (ticksUpperBound - ticksLowerBound) / range;
    }
    prevSunElevation = sunElevation;
  }
  DateTime sunset = DateTime(ticksLowerBound);
  return std::make_pair(sunrise, sunset);
}
