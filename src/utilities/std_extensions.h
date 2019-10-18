#ifndef COMPUTERMONITORINGSTATISTICSPARSER_STD_EXTENTIONS_H
#define COMPUTERMONITORINGSTATISTICSPARSER_STD_EXTENTIONS_H

#include <iostream>
#include <map>
#include "../useful_collections/time_container.h"

namespace std {
    template<typename A, typename B>
    pair<B, A> flip_pair(const std::pair<A, B> &p) {
        return std::pair<B, A>(p.second, p.first);
    }

    template<typename A, typename B>
    multimap<B, A> flip_map(const std::map<A, B> &src) {
        std::multimap<B, A> dst;
        std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()), flip_pair<A, B>);
        return dst;
    }
}

#endif //COMPUTERMONITORINGSTATISTICSPARSER_STD_EXTENTIONS_H
