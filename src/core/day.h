#ifndef COMPUTERMONITORINGSTATISTICSPARSER_DAY_H
#define COMPUTERMONITORINGSTATISTICSPARSER_DAY_H

#include <iostream>
#include <boost/date_time.hpp>

struct day {
    std::string day_name;
    boost::date_time::weekdays day_symbol;

    inline bool operator==(const day& ref) { return day_name == ref.day_name && day_symbol == ref.day_symbol; }
    inline bool operator==(const std::string& ref) { return day_name == ref; }
    inline bool operator==(const boost::date_time::weekdays& ref) { return day_symbol == ref; }
};

#endif //COMPUTERMONITORINGSTATISTICSPARSER_DAY_H
