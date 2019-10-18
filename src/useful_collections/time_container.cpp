#include "time_container.h"

size_t time_container::time_containers_count = 0;

time_container::time_container(TimeContainerLevel container_collection, bool is_percent, bool hidden, size_t order) :
        order(order), container_collection(container_collection), is_percent(is_percent), hidden(hidden) {
    time_containers_count++;
    if (order == 0) {
        this->order = time_containers_count;
    }
}

bool time_container::operator<(const time_container &ref) const {
    return order < ref.order;
}

bool time_container::operator>(const time_container &ref) const {
    return order > ref.order;
}

bool time_container::operator==(const time_container &ref) const {
    return order == ref.order;
}