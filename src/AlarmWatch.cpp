#include "AlarmWatch.cpp"

AlarmWatch::AlarmWatch()
{
  this->channelCount = 0;
  this->offset = 0;
  this->lcd = new LiquidCrystal(rs, en, d4, d5, d6, d7);
  this->lcd->begin(16, 2);
  lcd->print("Alarm Watch");
}

AlarmWatch::AlarmWatch(WeekTime startTime) : AlarmWatch()
{
  this->offset = startTime.GetTotalSeconds();
}

void AlarmWatch::AddAlarmChannel(AlarmChannel *alarmChannel)
{
  if (this->channelCount > sizeof(this->alarmChannels) - 1)
  {
    return;
  }
  this->alarmChannels[this->channelCount] = alarmChannel;
  this->channelCount++;
}

void AlarmWatch::Display(WeekTime weekTime)
{
  lcd->setCursor(0, 1);
  lcd->print(weekTime.ToString());
}

void AlarmWatch::Display(unsigned long relativSeconds)
{
  unsigned long totalSeconds = relativSeconds + this->offset;
  this->Display(WeekTime(totalSeconds));
  for (int i = 0; i < this->channelCount; i++)
  {
    this->alarmChannels[i]->Check(totalSeconds);
  }
}