#ifndef SUNRISESUNSETTIME_H
#define SUNRISESUNSETTIME_H
#include <utility>
#include <SGP4/DateTime.h>
#include <SGP4/Observer.h>
namespace SGP4
{
  std::pair< DateTime, DateTime > getSunriseAndSunsetTime(const DateTime &currTime, const Observer &observer);
}
#endif
