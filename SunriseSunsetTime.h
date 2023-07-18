#ifndef SUNRISESUNSETTIME_H
#define SUNRISESUNSETTIME_H
#include <utility>
#include <SGP4/DateTime.h>
#include <SGP4/Observer.h>
std::pair< SGP4::DateTime, SGP4::DateTime >
getSunriseAndSunsetTime(const SGP4::DateTime &currTime, const SGP4::Observer &observer);
#endif
