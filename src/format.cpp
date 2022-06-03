#include <string>

#include "format.h"

using std::string;



string Format::TimeToString(long time) {
  if (time < 10)
    return "0" + std::to_string(time);
  else
    return std::to_string(time);
}

string Format::ElapsedTime(long seconds) {
  string time = "00:00:00";
  long unit = 0;
  if (seconds > 0) {
    unit = seconds / 3600; //hours
    time = TimeToString(unit) + ":";
    unit = (seconds / 60) % 60; //mins
    time += TimeToString(unit) + ":";
    unit = seconds % 60; //seconds
    time += TimeToString(unit);
  }

  return time;
}

