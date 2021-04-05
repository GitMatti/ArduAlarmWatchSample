#include "WeekTime.h"
#include "AlarmChannel.h"
/// Einfacher Wecker mit bis zu 4 Alarmkanälen
class AlarmWatch
{
private:
  const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
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