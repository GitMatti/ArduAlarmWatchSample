# ArduAlarmWatchSample

Testprojekt zur Abbildung eines einfachen Weckers mit einem Arduino

```cpp
/// include the library code:
#include <LiquidCrystal.h>

// WeekTime.h
/// Primitiver Container zur zeitlichen Abbildung einer Woche
class WeekTime
{
public:
  unsigned int Second;
  unsigned int Minute;
  unsigned int Hour;
  unsigned int Day;

  /// Erzeugt eine WeekTime auf Basis der abgelaufenen Sekunden seit Montag 00:00 Uhr
  WeekTime(unsigned long totalSeconds);
  /// Erzeugt eine WeekTime an Hand von Wochentag, Stunde, Minute und Sekunde
  WeekTime(unsigned int day, unsigned int hour, unsigned int minute, unsigned int second);
  /// Liefert die Anzahl der vergangenen Sekunde seit Montag 00:00 Uhr
  unsigned long GetTotalSeconds();
  /// Liefert die Uhrzeit als Text im Format D HH:MM:SS
  String ToString();

private:
  const char *dayNames[7] =
      {"Mo", "Di", "Mi", "Do", "Fr", "Sa", "So"};

  String ToString(unsigned long value);
};

// WeekTime.cpp
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

// WeekTimeSpan.h

/// primitiver Container zur Abbildung einer Zeitspanne innerhalb einer Woche
class WeekTimeSpan
{
private:
  unsigned long start;
  unsigned long end;

public:
  /// Erzeugt eine leere Zeitspanne von MO 00:00 bis MO 00:00
  WeekTimeSpan();
  /// Erzeugt eine Zeitspanne aus Start- und Endzeit
  WeekTimeSpan(WeekTime startWeekTime, WeekTime endWeekTime);
  /// Erzeugt eine Zeitspanne aus Startzeit und Dauer in Sekunden
  WeekTimeSpan(WeekTime startWeekTime, unsigned long duration);
  /// Prüft, ob die übergebene Wochenzeitangabe innerhalb der definierten Zeitspanne liegt
  bool IsInRange(WeekTime weekTime);
  /// Prüft, ob der übergebene Moment innerhalb der definierten Zeitspanne liegt
  bool IsInRange(unsigned long totalSeconds);
};

// WeekTimeSpan.cpp

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
  Serial.println(start);
  Serial.println(end);
}

bool WeekTimeSpan::IsInRange(WeekTime weekTime)
{
  return this->IsInRange(weekTime.GetTotalSeconds());
}

bool WeekTimeSpan::IsInRange(unsigned long totalSeconds)
{
  return this->start <= totalSeconds && this->end >= totalSeconds;
}

// AlarmChannel.h

/// Container für einen Alarmkanal (Ausgabe-Pin)
/// Der Alarm kann bis zu 64 Alarmzeitspannen innerhalb einer Woche besitzen
class AlarmChannel
{
private:
  bool status = false;
  int alarmCount = 0;
  WeekTimeSpan alarms[64];
  int channel;

public:
  /// Legt einen neuen Alarm an und weist diesem dem Signal auf einem Ausgabeport zu
  AlarmChannel(int pin);
  /// Fügt ein weiteres Alarmzeitfenster hinzu
  void AddAlarm(WeekTimeSpan weekTimeSpan);
  /// Setzt den definierten Port auf true, wenn ein Alarmzeitfenster aktiv ist.
  /// Ist kein einzges Zeitfenster aktiv, wird der Port wieder auf false gesetzt
  void Check(WeekTime weekTime);
  void Check(unsigned long totalSeconds);

private:
  void SwitchOn();
  void SwitchOff();
};

// AlarmChannel.cpp

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

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

// AlarmWatch.h

/// Einfacher Wecker mit bis zu 4 Alarmkanälen
class AlarmWatch
{
private:
  LiquidCrystal *lcd;
  unsigned long offset;
  int channelCount;
  AlarmChannel *alarmChannels[4];

public:
  /// Erzeugt einen Wecker, der Mo 00:00 Uhr startet
  AlarmWatch();
  /// Erzeugt einen Wecker mir der angegebenen Wochenstartzeit
  AlarmWatch(WeekTime startTime);
  /// Fügt dem Wecker einen Alarm hinzu
  void AddAlarmChannel(AlarmChannel *alarm);
  /// Anzeigen der Wochenzeit und prüfen auf Alarmzustände
  void Display(WeekTime weekTime);
  /// Anzeigen der Wochenzeit (berechnet aus abgelaufener Zeit und Startzeit) und prüfen auf Alarmzustände
  void Display(unsigned long relativSeconds);
};

// AlarmWatch.cpp
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
  if (this->channelCounter > sizeof(this->alarmChannels) - 1)
  {
    return;
  }
  this->alarmChannels[this->channelCounter] = alarmChannel;
  this->channelCounter++;
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

/// Anlegen eines Weckers und Stellen der Zeit auf DO 12:02:10
AlarmWatch alarmWatch(WeekTime(3, 12, 2, 10));
// Anzahl der Milisekunden pro Sekunde
unsigned long divisor = 1000;

void setup()
{
  Serial.begin(9600);
  // Alarm für Ausgabe-Pin 10 anlegen
  AlarmChannel alarmChannel1 = AlarmChannel(10);
  // Startzeitpunkt für Alarm auf DO 12:02:13 setzen
  WeekTime startTime1 = WeekTime(3, 12, 2, 13);
  // Alarmzeit für Startzeitpunkt und 3 Sekunden Dauer anlegen
  WeekTimeSpan alarm1 = WeekTimeSpan(startTime1, 3);
  // Alarmzeit auf dem Alarm hinzufügen
  alarmChannel1.AddAlarm(alarm1);
  // Den Alarmkanal dem Wecker hinzufügen
  alarmWatch.AddAlarmChannel(&alarmChannel1);
}

void loop()
{
  unsigned long relativSeconds = millis() / divisor;
  alarmWatch.Display(relativSeconds);
}


```
