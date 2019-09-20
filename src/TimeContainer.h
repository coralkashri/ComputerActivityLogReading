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

struct TimeContainer {

    /// Constructors
    TimeContainer() = default;

    TimeContainer(TimeContainerLevel container_collection, bool is_percent = false, bool hidden = false, size_t order = 0);

    TimeContainer(const TimeContainer &ref) = default;

    TimeContainer &operator=(const TimeContainer &ref) = default;

    /// Member variables
    static size_t time_containers_count;
    size_t order;
    TimeContainerLevel container_collection;
    bool is_percent;
    bool hidden;
    boost::posix_time::time_duration duration;

    /// Functions
    bool operator<(const TimeContainer &ref) const;

    bool operator>(const TimeContainer &ref) const;

    bool operator==(const TimeContainer &ref) const;
};

#endif //COMPUTERMONITORINGSTATISTICSPARSER_TIMECONTAINER_H
