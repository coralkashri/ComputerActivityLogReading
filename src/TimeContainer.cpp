//
// Created by sherlock on 9/20/19.
//

#include "TimeContainer.h"

size_t TimeContainer::time_containers_count = 0;

TimeContainer::TimeContainer(TimeContainerLevel container_collection, bool is_percent, bool hidden, size_t order) :
        order(order), container_collection(container_collection), is_percent(is_percent), hidden(hidden) {
    time_containers_count++;
    if (order == 0) {
        this->order = time_containers_count;
    }
}

bool TimeContainer::operator<(const TimeContainer &ref) const {
    return order < ref.order;
}

bool TimeContainer::operator>(const TimeContainer &ref) const {
    return order > ref.order;
}

bool TimeContainer::operator==(const TimeContainer &ref) const {
    return order == ref.order;
}