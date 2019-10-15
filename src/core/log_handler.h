#ifndef COMPUTERMONITORINGSTATISTICSPARSER_LOG_HANDLER_H
#define COMPUTERMONITORINGSTATISTICSPARSER_LOG_HANDLER_H

#include <iostream>
#include <boost/date_time.hpp>
#include "time_container.h"
#include "variables.h"

class log_handler {
public:
    log_handler(bool analyze_verbose,
                bool analyze_process,
            const std::string &log_path,
            bool anomalies_verbose,
            const std::string &normal_login_word);

    void set_week_start_day(boost::date_time::weekdays day);

    void process() const;

private:
    void analyze() const;

    bool anomalies_detector() const;

    std::map<std::string, time_container> filter_times_container_by_level(TimeContainerLevel level, std::map<std::string, time_container> &durations) const;

    void show_statistics(TimeContainerLevel type, bool current, std::map<std::string, time_container> &durations) const;

    std::map<std::string, time_container> create_durations_container() const;

    base_variables init_variables() const;

    // Analyze variables
    bool analyze_verbose;
    bool analyze_process;
    std::string log_path;
    boost::date_time::weekdays week_start_day;

    // Anomalies variables
    bool anomalies_verbose;
    std::string normal_login_word;
};

#endif //COMPUTERMONITORINGSTATISTICSPARSER_LOG_HANDLER_H