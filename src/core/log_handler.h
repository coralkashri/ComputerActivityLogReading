#ifndef COMPUTERMONITORINGSTATISTICSPARSER_LOG_HANDLER_H
#define COMPUTERMONITORINGSTATISTICSPARSER_LOG_HANDLER_H

#include <iostream>
#include <boost/date_time.hpp>
#include "time_container.h"
#include "activity_analyzer.h"
#include "anomalies_detector.h"

class log_handler {
public:
    log_handler(bool analyze_verbose,
                bool enable_analyze,
            const std::string &log_path,
            bool anomalies_verbose,
            const std::string &normal_login_word);

    void config_analyzer_params(boost::date_time::weekdays week_start_day, float sleep_hours_per_day, float study_day_hours_in_week);

    void process();

private:

    activity_analyzer analyzer;
    anomalies_detector detector;
    bool enable_analyze;

};

#endif //COMPUTERMONITORINGSTATISTICSPARSER_LOG_HANDLER_H