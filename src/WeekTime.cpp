#include "WeekTime.h"

WeekTime::WeekTime(unsigned long totalSeconds)
{
  unsigned long totalMinutes = totalSeconds / 60ul;
  unsigned long totalHours = totalMinutes / 60ul;
  unsigned long totalDays = totalHours / 24ul;

  this->Second = totalSeconds % 60;
  this->Minute = totalMinutes % 60;
  this->Hour = totalHours % 24;
  this->Day = totalDays % 7;
}

WeekTime::WeekTime(unsigned int day, unsigned int hour, unsigned int minute, unsigned int second)
{
  this->Second = second;
  this->Minute = minute;
  this->Hour = hour;
  this->Day = day;
}

unsigned long WeekTime::GetTotalSeconds()
{
  return this->Second + 60ul * this->Minute + 3600ul * this->Hour + 86400ul * this->Day;
}

String WeekTime::ToString()
{
  return "Tag:" +
         String(dayNames[this->Day]) + " " +
         ToString(this->Hour) + ":" +
         ToString(this->Minute) + ":" +
         ToString(this->Second);
}

String WeekTime::ToString(unsigned long value)
{
  if (value < 10)
  {
    return "0" + String(value);
  }
  return String(value);
}