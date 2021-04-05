#include "WeekTime.h"

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