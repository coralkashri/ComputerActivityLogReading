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
#include "utilities/std_extentions.h"
#include "utilities/boost_extentions.h"
#include "utilities/design_text.h"
#include "TimeContainer.h"

using namespace std;
using namespace design_text;

map<string, TimeContainer> filter_times_container_by_level(TimeContainerLevel level, map<string, TimeContainer> &durations) {
    map<string, TimeContainer> filtered_durations;
    for_each(durations.begin(), durations.end(),
             [level, filtered = &filtered_durations](auto const &t) mutable {
                 if (t.second.container_collection == level) {
                     (*filtered)[t.first] = t.second;
                 }
             });
    return filtered_durations;
}

void show_statistics(TimeContainerLevel type, bool current, std::map<string, TimeContainer> &durations) {
    cout << endl;
    string type_string = [type]() -> string {
        switch (type) {
            case DAY:
                return "Days";
            case WEEK:
                return "Week";
            case MONTH:
                return "Month";
            case YEAR:
                return "Year";
        }
    }();
    type_string = current ? "Current " + type_string : type_string;
    cout << "~~~ " << type_string << " statistics ~~~" << endl;
    std::multimap<TimeContainer, string> sort_by_durations = flip_map<string, TimeContainer>(filter_times_container_by_level(type, durations));
    for (auto key : sort_by_durations) {
        if (!key.first.hidden) {
            cout << key.second << ": ";
            if (!key.first.is_percent) {
                cout << key.first.duration;
            } else {
                // Transform minutes into percents
                double percents = key.first.duration.total_seconds() / 60. / 60. * 100.;
                stringstream str;
                str << percents << "%";
                if (percents > 60) {
                    cout << make_colored(str.str(), Color::WHITE, Color::GREEN, true);
                } else if (percents > 50) {
                    cout << make_colored(str.str(), Color::GREEN, Color::NONE, true);
                } else {
                    cout << make_colored(str.str(), Color::RED, Color::NONE, true);
                }
            }
            cout << endl;
        }
        durations[key.second].duration -= durations[key.second].duration; // Reset duration into 00:00:00
    }
}

map<string, TimeContainer> create_durations_container() {

    /* *************************************************
     * TimeContainer
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

namespace po = boost::program_options;

int main(int ac, char* av[]) {
    po::options_description desc("Allowed options");
    bool verbose;
    desc.add_options()
            ("help", "produce help message")
            ("verbose", po::value<bool>(&verbose)->default_value(true), "Show detailed times of login")
            ;

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    string log_file_path = "/home/sherlock/message_from_computer"; /// Path to your log file
    ifstream log_file(log_file_path, ios::in);

    boost::posix_time::ptime start, stop;
    string datetime;

    map<string, TimeContainer> durations = create_durations_container();
    size_t study_day_hours_in_week = (boost::posix_time::time_duration(18,0,0) - boost::posix_time::time_duration(13,0,0)).hours();
    int last_week_number = -1, last_month_number = -1, last_year_number = -1, last_day_number = -1;

    /**
     * @param current_day - Counting from 1 => Sunday = 1, Monday = 2, etc.
     */
    auto end_week_calculations = [&durations, verbose, &stop, &start, study_day_hours_in_week] (size_t current_day) mutable {
        if (!verbose) {
            auto previous_start_date = (stop - (boost::posix_time::time_duration(24, 0, 0) * (current_day - 1)));
            if (stop.date().day_of_week() == start.date().day_of_week()) {
                previous_start_date -= boost::posix_time::time_duration(24, 0, 0);
            }
            if (current_day < 7) {
                previous_start_date += boost::posix_time::time_duration(24, 0, 0);
            }
            auto previous_stop_date = (previous_start_date + (boost::posix_time::time_duration(24, 0, 0) * 7));
            stringstream s;
            cout << "\n" << make_colored(stringstream() << previous_start_date.date() << " - " << previous_stop_date.date() << ":", Color::NONE, Color::CYAN, true);
        }

        float hours_of_sleep_per_day = 8.f;
        if (!current_day) current_day = 1; // Counting from 1 => Sunday = 1, Monday = 2, etc.

        /// Week
        // Calculate Study-day activity percents
        durations["Thursday hours percents from Study-day"].duration = durations["Thursday hours from Study-day"].duration / study_day_hours_in_week;
        // Calculate average daily activity
        durations["Daily average"].duration = durations["Week total"].duration / current_day;
        // Calculate average until Wed daily activity
        durations["Daily average until Wednesday"].duration = durations["Daily total until Wednesday"].duration / std::min(4, (int)current_day);
        // Calculate weekend percents
        durations["Weekend percents"].duration = durations["Weekend total"].duration / (int)(48.f - hours_of_sleep_per_day * 2.f);

        /// Month
        // Calculate average week percents in the current month
        durations["Month week percents average"].duration += durations["Weekend percents"].duration;
        // Calculate total month Study-day
        durations["Month Study-day total"].duration += durations["Thursday hours from Study-day"].duration;
    };

    /**
     * @param data ->
     *          day - current day in month
     *          month - current month
     *          year - current year
     */
    auto end_month_calculations = [&durations, study_day_hours_in_week, verbose] (boost::gregorian::date::ymd_type data) mutable {
        if (!verbose) {
            cout << "\n" << make_colored(stringstream()
                                                 << boost::gregorian::date(data.year, data.month, 1)
                                                 << " - "
                                                 << boost::gregorian::date(data).end_of_month() << ":",
                                         Color::NONE, Color::BLUE, true);
        }

        size_t current_week_in_month = (data.day - 1) / 7 + 1;
        // Calculate current month week average
        durations["Month week average"].duration = durations["Month total"].duration / current_week_in_month;
        // Finish calculate current month week percents average
        durations["Month week percents average"].duration /= current_week_in_month;
        // Calculate month Study-day usage percents
        durations["Month Study-day percents"].duration = durations["Month Study-day total"].duration /
                (study_day_hours_in_week * boost::date_time::how_much_specific_days_past_in_month(data.year, data.month, {boost::date_time::weekdays::Thursday}, data.day));
    };

    while (std::getline(log_file, datetime, '+')) {
        /// Read date-time from log
        log_file.ignore(256, '\n');
        start = boost::date_time::extract_datetime(datetime);

        if (last_month_number != -1) {
            /// Ignore calculations on the very first line in the file

            if ((start.date().day_of_week() == 0 && last_day_number != 0) || /// Sunday activity - new week
                (last_week_number != start.date().week_number() && start.date().day_of_week() != 0)) { /// New week without Sunday activity

                end_week_calculations(7);

                if (last_month_number == start.date().month()) {
                    show_statistics(DAY, false, durations);
                    show_statistics(WEEK, false, durations);
                } else { /// New month
                    size_t month_end_date = boost::gregorian::date(last_year_number, last_month_number, 1).end_of_month().day();
                    end_month_calculations({last_year_number, last_month_number, month_end_date});
                    show_statistics(MONTH, false, durations);
                }
                cout << endl;
            }
        }
        if (verbose)
            cout << start << " - ";
        boost::posix_time::time_duration difference;
        bool is_current;
        if (std::getline(log_file, datetime, '+')) {
            is_current = false;
            /// Read date-time from log
            log_file.ignore(256, '\n');
            stop = boost::date_time::extract_datetime(datetime);
        } else {
            is_current = true;
            /// Current point in time
            stop = boost::posix_time::second_clock::local_time();
        }
        difference = stop - start;
        if (!is_current) {
            if (verbose)
                cout << stop << ": " << difference << endl;
        } else {
            if (verbose)
                cout << "Now: " << difference << endl;
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

        last_week_number = start.date().week_number();
        last_day_number = start.date().day_of_week();
        if (last_day_number == 0) {
            last_week_number++;
        }
        last_month_number = start.date().month();
        last_year_number = start.date().year();
    }

    end_week_calculations(stop.date().day_of_week() + 1);
    show_statistics(DAY, true, durations);
    show_statistics(WEEK, true, durations);
    //if (last_month_number != start.date().month()) { /// New month
        end_month_calculations({last_year_number, last_month_number, stop.date().day()});
        show_statistics(MONTH, true, durations);
    //}

    return 0;
}