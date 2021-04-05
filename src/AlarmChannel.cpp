#include "AlarmChannel.h"

AlarmChannel::AlarmChannel(int outputPin)
{
  this->channel = outputPin;
  pinMode(this->channel, OUTPUT);
}

void AlarmChannel::AddAlarm(WeekTimeSpan alarm)
{
  if (this->alarmCount > sizeof(this->alarms) - 1)
  {
    return;
  }
  this->alarms[this->alarmCount] = alarm;
  this->alarmCount++;
}

void AlarmChannel::Check(WeekTime weekTime)
{
  Check(weekTime.GetTotalSeconds());
}

void AlarmChannel::Check(unsigned long totalSeconds)
{
  for (int i = 0; i < this->alarmCount; i++)
  {
    if (this->alarms[i].IsInRange(totalSeconds))
    {
      this->SwitchOn();
      return;
    }
  }
  this->SwitchOff();
}

void AlarmChannel::SwitchOn()
{
  if (this->status)
  {
    return;
  }
  this->status = true;
  digitalWrite(10, this->status);
}

void AlarmChannel::SwitchOff()
{
  if (!this->status)
  {
    return;
  }
  this->status = false;
  digitalWrite(10, this->status);
}