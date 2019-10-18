#ifndef COMPUTERMONITORINGSTATISTICSPARSER_TIMECONTAINER_H
#define COMPUTERMONITORINGSTATISTICSPARSER_TIMECONTAINER_H

#include <iostream>
#include <boost/date_time/posix_time/posix_time_config.hpp>

enum TimeContainerLevel {
    DAY = 0,
    WEEK = 1,
    MONTH = 2,
    YEAR = 3
};

struct time_container {

    /// Constructors
    time_container() = default;

    time_container(TimeContainerLevel container_collection, bool is_percent = false, bool hidden = false, size_t order = 0);

    time_container(const time_container &ref) = default;

    time_container &operator=(const time_container &ref) = default;

    /// Member variables
    static size_t time_containers_count; // Used for automatic order
    size_t order;
    TimeContainerLevel container_collection;
    bool is_percent;
    bool hidden;
    boost::posix_time::time_duration duration;

    /// Functions
    bool operator<(const time_container &ref) const;

    bool operator>(const time_container &ref) const;

    bool operator==(const time_container &ref) const;
};

#endif //COMPUTERMONITORINGSTATISTICSPARSER_TIMECONTAINER_H
