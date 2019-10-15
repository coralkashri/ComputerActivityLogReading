#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>
#include "core/day.h"
#include "core/log_handler.h"
#include "utilities/design_text.h"
#include "cmd_options.h"

int main(int ac, char* av[]) {
    cmd_options command_line_options(ac, av);
    cmd_options_data cmd_data = command_line_options.get_data();

    /// --help / -h option handler
    if (cmd_data.help) {
        std::cout << cmd_data.visible_options << "\n";
        return EXIT_SUCCESS;
    }

    /// --log-path / -l option handler
    if (!boost::filesystem::exists(cmd_data.log_file_path))
        throw std::runtime_error("Log file path doesn't exist.");

    /// --week-start-day / -d option handler
    /// Initialize available days list
    auto available_days = std::vector<day>{{"sunday", boost::date_time::weekdays::Sunday},
                           {"monday", boost::date_time::weekdays::Monday},
                           {"tuesday", boost::date_time::weekdays::Tuesday},
                           {"wednesday", boost::date_time::weekdays::Wednesday},
                           {"thursday", boost::date_time::weekdays::Thursday},
                           {"friday", boost::date_time::weekdays::Friday},
                           {"saturday", boost::date_time::weekdays::Saturday}};
    if (auto selected_day = std::find(available_days.begin(), available_days.end(), boost::to_lower_copy(cmd_data.week_start_day)); selected_day != available_days.end()) { // Selected day exists
        log_handler::week_start_day = selected_day->day_symbol;
    } else { // Selected day doesn't exists
        if (cmd_data.week_start_day == "help") { // Produce help days message
            std::cout << "Available days:" << std::endl;
            std::cout << "\tSun [Sunday]" << std::endl;
            std::cout << "\tMon [Monday]" << std::endl;
            std::cout << "\tTue [Tuesday]" << std::endl;
            std::cout << "\tWed [Wednesday]" << std::endl;
            std::cout << "\tThu [Thursday]" << std::endl;
            std::cout << "\tFri [Friday]" << std::endl;
            std::cout << "\tSat [Saturday]" << std::endl;
            return EXIT_SUCCESS;
        }
        throw std::runtime_error("Unfamiliar day, for options list use '-d [ --week-start-day ] help'.");
    }

    // Anomalies detector
    auto anomaly_detected = log_handler::anomalies_detector(cmd_data.log_file_path, cmd_data.normal_login_word, cmd_data.anomaly_detection);
    if (cmd_data.analyze_activity) // Analyze logger times
        log_handler::analyze(cmd_data.log_file_path, cmd_data.verbose);
    if (anomaly_detected) // Produce anomalies warning if needed
        std::cout << "\n\n" << design_text::make_colored(std::stringstream() << "*** Anomaly detected! ***", design_text::Color::NONE, design_text::Color::RED, true) << std::endl;

    return EXIT_SUCCESS;
}