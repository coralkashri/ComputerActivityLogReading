#ifndef COMPUTERMONITORINGSTATISTICSPARSER_VARIABLES_H
#define COMPUTERMONITORINGSTATISTICSPARSER_VARIABLES_H

#include <iostream>
#include <boost/date_time.hpp>

struct base_variables {
    u_short study_day_hours_in_week;
    u_short sleep_hours_per_day;
    u_short hours_per_day;
    boost::date_time::weekdays week_start_day;
};

#endif //COMPUTERMONITORINGSTATISTICSPARSER_VARIABLES_H