#include "day.h"

day::day(const std::string &day_name, const std::string &short_day_name, boost::date_time::weekdays day_symbol)
        : day_name(day_name), short_day_name(short_day_name), day_symbol(day_symbol) {}

day::day(const std::string &day_name, boost::date_time::weekdays day_symbol)
        : day_name(day_name), short_day_name(day_name.substr(0, 3)), day_symbol(day_symbol) {}