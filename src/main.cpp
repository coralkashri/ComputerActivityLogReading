#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace std;

namespace boost {
    namespace date_time {
        posix_time::ptime parse_dmy_time(const std::string &s, char sep) {
            typedef typename posix_time::ptime::time_duration_type time_duration;
            typedef typename posix_time::ptime::date_type date_type;

            //split date/time on a unique delimiter char such as ' ' or 'T'
            std::string date_string, tod_string;
            split(s, sep, date_string, tod_string);
            //call parse_date with first string
            date_type d = parse_date<date_type>(date_string, ymd_order_dmy);
            //call parse_time_duration with remaining string
            time_duration td = parse_delimited_time_duration<time_duration>(tod_string);
            //construct a time
            return {d, td};
        }
    }
}

boost::posix_time::ptime extract_datetime(const string &datetime_s) {
    string tmp = datetime_s;
    tmp = datetime_s.substr(0, datetime_s.size() - 1);
    boost::posix_time::ptime datetime(boost::date_time::parse_dmy_time(tmp, ' '));
    return datetime;
}

struct TimeContainer {
    size_t order;
    bool is_percent;
    boost::posix_time::time_duration duration;

    bool operator<(const TimeContainer& ref) const {
        return order < ref.order;
    }

    bool operator>(const TimeContainer& ref) const {
        return order > ref.order;
    }

    bool operator==(const TimeContainer& ref) const {
        return order == ref.order;
    }
};

template<typename A, typename B>
std::pair<B,A> flip_pair(const std::pair<A,B> &p)
{
    return std::pair<B,A>(p.second, p.first);
}

template<typename A, typename B>
std::multimap<B,A> flip_map(const std::map<A,B> &src)
{
    std::multimap<B,A> dst;
    std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()),
                   flip_pair<A,B>);
    return dst;
}

void show_statistics(const string &type, std::map<const string, TimeContainer> &durations) {
    cout << endl << endl;
    cout << "~~~ " << type << " statistics ~~~" << endl;
    std::multimap<TimeContainer, const string> sort_by_durations = flip_map<const string, TimeContainer>(durations);
    for (auto key : sort_by_durations) {
        cout << key.second << ": ";
        if (!key.first.is_percent) {
             cout << key.first.duration;
        } else {
            cout << key.first.duration.total_seconds() / 60. / 60. * 100. << "%";
        }
        cout << endl;
        durations[key.second].duration -= durations[key.second].duration;
    }
    cout << endl << endl;
}

int main() {
    string log_file_path = "/path/to/example_log.txt";
    ifstream log_file(log_file_path, ios::in);

    boost::posix_time::ptime start, stop;
    string datetime;

    map<const string, TimeContainer> week_durations = {
            {"Daily average", {0}},
            {"Daily average until Wednesday", {1}},
            {"Daily total until Wednesday", {2}},
            {"Thursday total", {3}},
            {"Thursday hours from Study-day", {4}},
            {"Thursday hours percents from Study-day", {5, true}},
            {"Weekend total", {6}},
            {"Weekend percents", {7, true}},
            {"Week total", {8}},
    };
    int last_week_number = -1, last_month_number = -1, last_day_number = -1;

    auto end_week_calculations = [&week_durations] () {
        float hours_of_sleep_per_day = 8.f;

        // Calculate Study-day activity percents
        week_durations["Thursday hours percents from Study-day"].duration = week_durations["Thursday hours from Study-day"].duration / (boost::posix_time::time_duration(18,0,0) - boost::posix_time::time_duration(13,0,0)).hours();
        // Calculate average daily activity
        week_durations["Daily average"].duration = week_durations["Week total"].duration / 7;
        // Calculate average until Wed daily activity
        week_durations["Daily average until Wednesday"].duration = week_durations["Daily total until Wednesday"].duration / 4;
        // Calculate weekend percents
        week_durations["Weekend percents"].duration = week_durations["Weekend total"].duration / (int)(48.f - hours_of_sleep_per_day * 2.f);
    };
    while (std::getline(log_file, datetime, '+')) {
        /// Read date-time from log
        log_file.ignore(256, '\n');
        start = extract_datetime(datetime);

        if (last_month_number != -1) {
            /// Ignore calculations on the very first line in the file

            if ((start.date().day_of_week() == 0 && last_day_number != 0) || /// Sunday activity - new week
                (last_week_number != start.date().week_number() && start.date().day_of_week() != 0)) {
                /// New week without Sunday activity

                end_week_calculations();
                show_statistics("Week", week_durations);
            }

            if (last_month_number != start.date().month()) { /// New month

            }
        }

        cout << start << " - ";
        boost::posix_time::time_duration difference;
        if (std::getline(log_file, datetime, '+')) {
            /// Read date-time from log
            log_file.ignore(256, '\n');
            stop = extract_datetime(datetime);
            difference = stop - start;
            cout << stop << ": " << difference << endl;
        } else {
            /// Current point in time
            stop = boost::posix_time::second_clock::local_time();
            difference = stop - start;
            cout << "Now: " << difference << endl;
            // After the `while-loop` the current statistics will be shown
        }

        if (start.date().day_of_week() < 4) {
            /// Until thursday
            week_durations["Daily total until Wednesday"].duration += difference;
        }
        if (start.date().day_of_week() == 4) {
            /// Thursday
            week_durations["Thursday total"].duration += difference;
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
                week_durations["Thursday hours from Study-day"].duration += actual_study_stop - actual_study_start;
            }

        }
        if (start.date().day_of_week() > 4) {
            /// Friday - Saturday
            week_durations["Weekend total"].duration += difference;
        }
        week_durations["Week total"].duration += difference;

        last_week_number = start.date().week_number();
        last_day_number = start.date().day_of_week();
        if (last_day_number == 0) {
            last_week_number++;
        }
        last_month_number = start.date().month();
    }
    end_week_calculations();
    show_statistics("Current week", week_durations);

    return 0;
}