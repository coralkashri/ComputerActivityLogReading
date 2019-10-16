#ifndef COMPUTERMONITORINGSTATISTICSPARSER_ACTIVITY_ANALYZER_H
#define COMPUTERMONITORINGSTATISTICSPARSER_ACTIVITY_ANALYZER_H

#include <iostream>
#include "variables.h"
#include "time_container.h"

class activity_analyzer {
public:

    explicit activity_analyzer(bool verbose, const std::string &log_path);

    /**
     * Flow:  -------------------------------------------------------------------------------------------------------------------
     *        |                                                                                                                 |
     *        V                                                                                                                 |
     * ------------------           -------------       ---------------------           -------------------------------------   |
     * New line in file | --true--> | Get start | ----> | start is new week | --true--> | update_durations_week_ending(7)   |   |
     * ------------------           -------------       ---------------------           | update_durations_month_ending(..) |   |
     *       |                                                 |                        -------------------------------------   |
     *       |                                                 |                            /                                   |
     *     false                                             false        ,----------------"                                    |
     *       |                                                 |          |                                                     |
     *       V                                                 V          V                                                     |
     * --------------------------                     --------------------------       -----------------------                  |
     * update durations current |                     | Update start_week_date | ----> | Calculate durations | ----> -----------|
     *       Week & Month       |                     |      & Get stop        |       -----------------------
     * --------------------------                     --------------------------
     */
    void analyze() const;

    void config_analyze_params(boost::date_time::weekdays week_start_day, float sleep_hours_per_day, float study_day_hours_in_week);

private:

    std::map<std::string, time_container> filter_times_container_by_level(TimeContainerLevel level, std::map<std::string, time_container> &durations) const;

    void show_statistics(TimeContainerLevel type, bool current, std::map<std::string, time_container> &durations) const;

    std::map<std::string, time_container> create_durations_container() const;

    void init_global_world_variables();

    /**
     *
     * @param durations - durations map to update
     * @param start_week_date - the date the week started on
     * @param current_day - Counting from 1 => Sunday = 1, Monday = 2, etc.
     */
    void update_durations_week_ending(std::map<std::string, time_container> &durations, boost::posix_time::ptime start_week_date, size_t current_day) const ;

     /**
      *
      * @param durations - durations map to update
      * @param data ->
     *          day - current day in month
     *          month - current month
     *          year - current year
     */
    void update_durations_month_ending(std::map<std::string, time_container> &durations, boost::gregorian::date::ymd_type data) const ;

    std::string log_path;
    base_variables analyze_properties;

    // Output streamers
    std::ostream &detailed_streamer;
    std::ostream &compressed_streamer;
    std::ostream temp_streamer;
};

#endif //COMPUTERMONITORINGSTATISTICSPARSER_ACTIVITY_ANALYZER_H