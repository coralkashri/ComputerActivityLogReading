#ifndef COMPUTERMONITORINGSTATISTICSPARSER_CMD_OPTIONS_PROGRESS_DATA_H
#define COMPUTERMONITORINGSTATISTICSPARSER_CMD_OPTIONS_PROGRESS_DATA_H

#include <boost/date_time.hpp>

struct cmd_options_progress_data {
    explicit cmd_options_progress_data() {}
    boost::date_time::weekdays week_start_day;
};

#endif //COMPUTERMONITORINGSTATISTICSPARSER_CMD_OPTIONS_PROGRESS_DATA_H