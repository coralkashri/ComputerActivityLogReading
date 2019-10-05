//
// Created by sherlock on 9/20/19.
//

#ifndef COMPUTERMONITORINGSTATISTICSPARSER_BOOST_EXTENTIONS_H
#define COMPUTERMONITORINGSTATISTICSPARSER_BOOST_EXTENTIONS_H

#include <boost/date_time/posix_time/ptime.hpp>

namespace boost {
    namespace date_time {
        /**
         * @brief Parse d/m/y h:m:s string format into boost::posix_time::ptime object
         *
         * @param s - date-time string with d/m/y h:m:s format
         * @param sep - The separate char between the date and time (For example: 'd/m/y h:m:s' => ' ').
         * @return posix_time::ptime object after parsing.
         */
        posix_time::ptime parse_dmy_time(const std::string &s, char sep);

        posix_time::ptime extract_datetime(std::string &datetime_s);

        /**
         * @brief Counts how much days of specific type (Sunday, Monday, etc.) passed in the current month untill the specified day
         *
         * @param year - Target year to check in
         * @param month - Target month to check in
         * @param days_to_count - Days types to count (Sunday, Monday, etc.)
         * @param day_in_month - Last day to check in the target month (If specified, stop counting in specified day in month)
         *
         * @return How much wanted days passed until day_in_month in the specified month and year
         */
        size_t how_much_specific_days_past_in_month(size_t year, size_t month, std::vector<weekdays> days_to_count, size_t day_in_month = 32);
    }
}

#endif //COMPUTERMONITORINGSTATISTICSPARSER_BOOST_EXTENTIONS_H