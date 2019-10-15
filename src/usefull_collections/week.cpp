#include "week.h"
#include "../utilities/exceptions.h"

week::week() {
    initialize_days_list();
}

std::string week::get_textual_days_list() const {
    std::stringstream ss;
    ss << "Available days:\n";
    ss << "\tSun [Sunday]\n";
    ss << "\tMon [Monday]\n";
    ss << "\tTue [Tuesday]\n";
    ss << "\tWed [Wednesday]\n";
    ss << "\tThu [Thursday]\n";
    ss << "\tFri [Friday]\n";
    ss << "\tSat [Saturday]" << std::endl;
    return ss.str();
}

day week::get_day(const std::string &day_name) const {
    if (auto selected_day = std::find(days_list.begin(), days_list.end(), boost::to_lower_copy(day_name));
            selected_day != days_list.end()) { // Selected day exists
        return *selected_day;
    }
    if (day_name == "help") {
        std::cout << *this << std::endl;
        throw exceptions::HelpMessageProducedException();
    }
    throw exceptions::DayNotFoundException();
}

void week::initialize_days_list() {
    days_list = {
            {"sunday",    boost::date_time::weekdays::Sunday},
            {"monday",    boost::date_time::weekdays::Monday},
            {"tuesday",   boost::date_time::weekdays::Tuesday},
            {"wednesday", boost::date_time::weekdays::Wednesday},
            {"thursday",  boost::date_time::weekdays::Thursday},
            {"friday",    boost::date_time::weekdays::Friday},
            {"saturday",  boost::date_time::weekdays::Saturday}
    };
}

std::ostream& operator<<(std::ostream& cout, const week& week_ref) {
    return cout << week_ref.get_textual_days_list();
}