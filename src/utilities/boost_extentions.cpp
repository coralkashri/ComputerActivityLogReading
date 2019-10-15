#include "boost_extentions.h"
#include <boost/date_time/time_parsing.hpp>

namespace boost {
    namespace date_time {
        posix_time::ptime parse_dmy_time(const std::string &s, char sep) {
            typedef typename posix_time::ptime::time_duration_type time_duration;
            typedef typename posix_time::ptime::date_type date_type;

            //split date/time on a unique delimiter char such as ' ' or 'T'
            std::string date_string, tod_string;
            split(s, sep, date_string, tod_string);
            //call parse_date with first string
            auto d = parse_date<date_type>(date_string, ymd_order_dmy);
            //call parse_time_duration with remaining string
            auto td = parse_delimited_time_duration<time_duration>(tod_string);
            //construct a time
            return {d, td};
        }

        posix_time::ptime extract_datetime(std::string &datetime_s) {
            std::string tmp = datetime_s;
            tmp = datetime_s.substr(0, datetime_s.size() - 1);
            posix_time::ptime datetime(parse_dmy_time(tmp, ' '));
            return datetime;
        }

        size_t how_much_specific_days_past_in_month(size_t year, size_t month, std::vector<weekdays> days_to_count, size_t day_in_month) {
            size_t res = 0;
            gregorian::date d(year, month, 1);
            for (size_t day = 1; day <= day_in_month; day++) {
                if (find(days_to_count.begin(), days_to_count.end(), d.day_of_week()) != days_to_count.end())
                    res++;
                d += gregorian::days(1);
            }
            return res;
        }
    }
}