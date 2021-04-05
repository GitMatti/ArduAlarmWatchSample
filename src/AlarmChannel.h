#include "WeekTime.h"
#include "WeekTimeSpan.h"

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