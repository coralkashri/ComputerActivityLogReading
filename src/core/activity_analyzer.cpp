#include "activity_analyzer.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include "../utilities/std_extensions.h"
#include "../utilities/boost_extensions.h"
#include "../utilities/design_text.h"

using namespace design_text;
using time_var = boost::posix_time::ptime;

activity_analyzer::activity_analyzer(bool verbose, const std::string &log_path)
        :
        temp_streamer(nullptr),
        compressed_streamer(verbose ? temp_streamer : std::cout),
        detailed_streamer(verbose ? std::cout : temp_streamer),
        log_path(log_path) {
    init_global_world_variables();
}

std::map<std::string, time_container> activity_analyzer::filter_times_container_by_level(TimeContainerLevel level, std::map<std::string, time_container> &durations) const {
    std::map<std::string, time_container> filtered_durations;
    for_each(durations.begin(), durations.end(),
             [level, filtered = &filtered_durations](auto const &t) mutable {
                 if (t.second.container_collection == level) {
                     (*filtered)[t.first] = t.second;
                 }
             });
    return filtered_durations;
}

void activity_analyzer::show_statistics(TimeContainerLevel type, bool current, std::map<std::string, time_container> &durations) const {
    std::cout << std::endl;
    std::string type_string = [type]() -> std::string {
        switch (type) {
            case DAY:
                return "Days";
            case WEEK:
                return "Week";
            case MONTH:
                return "Month";
            case YEAR:
                return "Year";
            default:
                return "Unknown";
        }
    }();
    type_string = current ? "Current " + type_string : type_string;
    std::cout << "~~~ " << type_string << " statistics ~~~" << std::endl;
    std::multimap<time_container, std::string> sort_by_durations = std::flip_map<std::string, time_container>(filter_times_container_by_level(type, durations));
    for (auto key : sort_by_durations) {
        if (!key.first.hidden) {
            std::cout << key.second << ": ";
            if (!key.first.is_percent) {
                std::cout << key.first.duration;
            } else {
                // Transform minutes into percents
                double percents = key.first.duration.total_seconds() / 60. / 60. * 100.;
                std::stringstream str;
                str << percents << "%";
                if (percents > 60) {
                    std::cout << make_colored(str.str(), Color::WHITE, Color::GREEN, true);
                } else if (percents > 50) {
                    std::cout << make_colored(str.str(), Color::GREEN, Color::NONE, true);
                } else {
                    std::cout << make_colored(str.str(), Color::RED, Color::NONE, true);
                }
            }
            std::cout << std::endl;
        }
        durations[key.second].duration -= durations[key.second].duration; // Reset duration into 00:00:00
    }
}

std::map<std::string, time_container> activity_analyzer::create_durations_container() const {

    /* *************************************************
     * time_container
     * Constructor:
     * @param TimeContainerLevel container_collection   [required - DAY | WEEK | MONTH | YEAR]
     * @param bool is_percent                           [optional - default false]
     * @param bool hidden                               [optional - default false]
     * @param size_t order                              [optional - default 0]
     * *************************************************/

    return {
            // Description                              Type    %?  Hidden
            {"Daily average",                          {DAY}},
            {"Daily average until Wednesday",          {WEEK}},
            {"Daily total until Wednesday",            {WEEK}},
            {"Thursday total",                         {DAY}},
            {"Thursday hours from Study-day",          {DAY}},
            {"Thursday hours percents from Study-day", {DAY,   true}},
            {"Weekend total",                          {DAY}},
            {"Weekend percents",                       {DAY,   true}},
            {"Week total",                             {WEEK}},
            {"Week percents",                          {WEEK,  true, true}}, // Currently inactive todo: Think how to calculate week usage percents
            {"Month week average",                     {MONTH}},
            {"Month week percents average",            {MONTH, true, true}}, // Currently inactive todo: Think how to calculate week usage percents
            {"Month Study-day total",                  {MONTH}},
            {"Month Study-day percents",               {MONTH, true}},
            {"Month total",                            {MONTH}},
            {"Year week average",                      {YEAR}},
            {"Year month average",                     {YEAR}},
            {"Year total",                             {YEAR}},
    };
}

void activity_analyzer::init_global_world_variables() {
    analyze_properties.hours_per_day = 24;
}

void activity_analyzer::update_durations_week_ending(std::map<std::string, time_container> &durations, boost::posix_time::ptime start_week_date, size_t current_day) const {
    boost::posix_time::ptime end_week_date = start_week_date + (boost::posix_time::time_duration(24, 0, 0) * 7);
    compressed_streamer << "\n" << make_colored(std::stringstream()
                                              << start_week_date.date()
                                              << " - "
                                              << end_week_date.date() << ":",
                                      Color::NONE, Color::CYAN, true);

    if (!current_day) current_day = 1; // Counting from 1 => Sunday = 1, Monday = 2, etc.

    /// Week
    // Calculate Study-day activity percents
    durations["Thursday hours percents from Study-day"].duration = durations["Thursday hours from Study-day"].duration / analyze_properties.study_day_hours_in_week;
    // Calculate average daily activity
    durations["Daily average"].duration = durations["Week total"].duration / current_day;
    // Calculate average until Wed daily activity
    durations["Daily average until Wednesday"].duration = durations["Daily total until Wednesday"].duration / std::min(4, (int) current_day);
    // Calculate weekend percents
    durations["Weekend percents"].duration = durations["Weekend total"].duration / ((analyze_properties.hours_per_day - analyze_properties.sleep_hours_per_day) * 2);

    /// Month
    // Calculate average week percents in the current month
    durations["Month week percents average"].duration += durations["Weekend percents"].duration;
    // Calculate total month Study-day
    durations["Month Study-day total"].duration += durations["Thursday hours from Study-day"].duration;
}

void activity_analyzer::update_durations_month_ending(std::map<std::string, time_container> &durations, boost::gregorian::date::ymd_type data) const {
    compressed_streamer << "\n" << make_colored(std::stringstream()
                                              << boost::gregorian::date(data.year, data.month, 1)
                                              << " - "
                                              << boost::gregorian::date(data) << ":",
                                      Color::NONE, Color::BLUE, true);

    size_t current_week_in_month = (data.day - 1) / 7 + 1;
    // Calculate current month week average
    durations["Month week average"].duration = durations["Month total"].duration / current_week_in_month;
    // Finish calculate current month week percents average
    durations["Month week percents average"].duration /= current_week_in_month;
    // Calculate month Study-day usage percents
    u_short study_days_until_now_in_month = boost::date_time::how_much_specific_days_past_in_month(data.year, data.month, {boost::date_time::weekdays::Thursday}, data.day);
    u_short study_day_hours_in_month = analyze_properties.study_day_hours_in_week * study_days_until_now_in_month;
    durations["Month Study-day percents"].duration = durations["Month Study-day total"].duration / study_day_hours_in_month;
}

void activity_analyzer::analyze() const {
    std::ifstream log_file(log_path, std::ios::in);

    time_var start, stop;
    time_var start_week_date;
    time_var end_week_date;
    std::string datetime;

    std::map<std::string, time_container> durations = create_durations_container();
    u_short last_month_number;
    u_short last_year_number;
    last_month_number = last_year_number = 0;
    bool is_first_line = true;

    auto update_week_start_stop_date = [&start, wsd = analyze_properties.week_start_day, &start_week_date, &end_week_date] {
        // Move the previous_start_date to the start of the week (if the user was inactive during the very first day of the week).
        start_week_date = start - boost::posix_time::time_duration(24, 0, 0) *
                                  (start.date().day_of_week() - wsd + (start.date().day_of_week() - wsd < 0 ? 7 : 0));
        end_week_date = start_week_date + boost::posix_time::time_duration(24, 0, 0) * 7; // Calculate week's end date
    };

    while (std::getline(log_file, datetime, '+')) {
        /// Read date-time from log
        log_file.ignore(256, '\n');
        start = boost::date_time::extract_datetime(datetime);

        if (!is_first_line) { /// Ignore calculations on the very first line in the file
            if (start.date() >= end_week_date.date()) {
                update_durations_week_ending(durations, start_week_date, 7);

                if (last_month_number == start.date().month()) {
                    show_statistics(DAY, false, durations);
                    show_statistics(WEEK, false, durations);
                } else { /// New month
                    u_short month_end_date = boost::gregorian::date(last_year_number, last_month_number, 1).end_of_month().day();
                    update_durations_month_ending(durations, {last_year_number, last_month_number, month_end_date});
                    show_statistics(DAY, false, durations);
                    show_statistics(WEEK, false, durations);
                    show_statistics(MONTH, false, durations);
                }
                update_week_start_stop_date();
                std::cout << std::endl;
            }
        } else {
            update_week_start_stop_date();
        }
        detailed_streamer << start << " - ";
        boost::posix_time::time_duration difference;
        bool is_current; // Is current point in time
        if (std::getline(log_file, datetime, '+')) {
            is_current = false;
            /// Read date-time from log
            log_file.ignore(256, '\n');
            stop = boost::date_time::extract_datetime(datetime);
        } else {
            is_current = true;
            // Current point in time
            stop = boost::posix_time::second_clock::local_time();
        }
        difference = stop - start;
        if (!is_current) {
            detailed_streamer << stop << ": " << difference << std::endl;
        } else {
            detailed_streamer << "Now: " << difference << std::endl;
            // After the `while-loop` the current statistics will be shown
        }

        if (start.date().day_of_week() < 4) {
            /// Until thursday
            durations["Daily total until Wednesday"].duration += difference;
        }
        if (start.date().day_of_week() == 4) {
            /// Thursday
            durations["Thursday total"].duration += difference;
            boost::posix_time::ptime studyday_start(start.date()), studyday_stop(start.date());
            studyday_start += boost::posix_time::time_duration(13, 0, 0);
            studyday_stop += boost::posix_time::time_duration(18, 0, 0);

            if (start < studyday_stop && stop > studyday_start) {
                /// Study-day range
                boost::posix_time::ptime actual_study_start, actual_study_stop;
                if (studyday_start - start >= boost::posix_time::time_duration(0, 0, 0)) {
                    actual_study_start = studyday_start;
                } else {
                    actual_study_start = start;
                }

                if (studyday_stop - stop <= boost::posix_time::time_duration(0, 0, 0)) {
                    actual_study_stop = studyday_stop;
                } else {
                    actual_study_stop = stop;
                }
                // Calculate total week Study-day
                durations["Thursday hours from Study-day"].duration += actual_study_stop - actual_study_start;
            }
        }
        if (start.date().day_of_week() > 4) {
            /// Friday - Saturday
            durations["Weekend total"].duration += difference;
        }
        durations["Week total"].duration += difference;
        durations["Month total"].duration += difference;

        last_month_number = start.date().month();
        last_year_number = start.date().year();
        is_first_line = false;
    }

    update_durations_week_ending(durations, start_week_date, stop.date().day_of_week() + 1);
    show_statistics(DAY, true, durations);
    show_statistics(WEEK, true, durations);
    update_durations_month_ending(durations, {last_year_number, last_month_number, stop.date().day()});
    show_statistics(MONTH, true, durations);
}

void activity_analyzer::config_analyze_params(boost::date_time::weekdays week_start_day, u_short sleep_hours_per_day, u_short study_day_hours_in_week) {
    analyze_properties.study_day_hours_in_week = study_day_hours_in_week;
    analyze_properties.sleep_hours_per_day = sleep_hours_per_day;
    analyze_properties.week_start_day = week_start_day;
}
