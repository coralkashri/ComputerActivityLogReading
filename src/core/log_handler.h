#ifndef COMPUTERMONITORINGSTATISTICSPARSER_LOG_HANDLER_H
#define COMPUTERMONITORINGSTATISTICSPARSER_LOG_HANDLER_H

#include <iostream>
#include <boost/date_time.hpp>

namespace log_handler {
    extern boost::date_time::weekdays week_start_day;
    void analyze(std::string log_path, bool verbose = false);
    bool anomalies_detector(std::string log_path, std::string normal_login_word, bool show_details);
};


#endif //COMPUTERMONITORINGSTATISTICSPARSER_LOG_HANDLER_H
