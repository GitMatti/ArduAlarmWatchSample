#include "WeekTimeSpan.h"

WeekTimeSpan::WeekTimeSpan()
{
}

WeekTimeSpan::WeekTimeSpan(WeekTime startWeekTime, WeekTime endWeekTime)
{
  start = startWeekTime.GetTotalSeconds();
  end = endWeekTime.GetTotalSeconds();
}

WeekTimeSpan::WeekTimeSpan(WeekTime startWeekTime, unsigned long duration)
{
  start = startWeekTime.GetTotalSeconds();
  end = start + duration;
}

bool WeekTimeSpan::IsInRange(WeekTime weekTime)
{
  return this->IsInRange(weekTime.GetTotalSeconds());
}

bool WeekTimeSpan::IsInRange(unsigned long totalSeconds)
{
  return this->start <= totalSeconds && this->end >= totalSeconds;
}