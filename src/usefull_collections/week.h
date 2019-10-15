#ifndef COMPUTERMONITORINGSTATISTICSPARSER_WEEK_H
#define COMPUTERMONITORINGSTATISTICSPARSER_WEEK_H

#include <iostream>
#include "day.h"

class week {
public:
    /**
     * Ctor
     *
     * @brief Initialize days list
     */
    explicit week();

    ~week() {}

    /**
     * @brief Prints days list
     */
    std::string get_textual_days_list() const;

    /**
     * @brief Get day object that represent the given day_name
     *
     * @param day_name - Day's full / short name.
     *
     * @note non-case sensitivity
     *
     * @throw DayNotFoundException - if given day not found in days_list.
     *
     * @return day object of the given string day
     */
    day get_day(const std::string &day_name) const;

private:
    void initialize_days_list();

    std::vector<day> days_list;
};

std::ostream& operator<<(std::ostream& cout, const week& week_ref);

#endif //COMPUTERMONITORINGSTATISTICSPARSER_WEEK_H