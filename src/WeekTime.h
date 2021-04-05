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