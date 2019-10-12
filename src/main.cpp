#include <iostream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>
#include "core/day.h"
#include "core/log_handler.h"
#include "utilities/design_text.h"

using namespace std;
namespace po = boost::program_options;

int main(int ac, char* av[]) {
    /// Command line options initialize
    po::options_description visible_desc("Usage: program [options] [path/]logger_filename");
    po::options_description full_desc;
    po::positional_options_description pd;
    bool verbose, anomaly_detection, analyze_activity;
    string normal_login_word;
    string log_file_path;
    string week_start_day;

    auto generic_group = po::options_description("Generic options");
    generic_group.add_options()
            ("help,h", "produce help message")
            ("verbose", po::value<bool>(&verbose)->default_value(false), "Show detailed times of login.")
            ;

    auto calender_group = po::options_description("Calender options");
    calender_group.add_options()
            ("week-start-day,d", po::value<string>(&week_start_day)->default_value("Monday"), "Week starting day ('--week-start-day help' for a list).")
            ;

    auto log_group = po::options_description("Logger options");
    log_group.add_options()
            ;

    auto hidden_options_group = po::options_description("Logger options");
    hidden_options_group.add_options()
            ("log-path,l", po::value<string>(&log_file_path)->default_value("/home/sherlock/message_from_computer"), "Path to login/logout logger.")
            ;

    auto anomalies_group = po::options_description("Mode options");
    anomalies_group.add_options()
            ("analyze-log", po::value<bool>(&analyze_activity)->default_value(true), "Analyze activity - show activity times and summarise activity.")
            ("anomaly-detection", po::value<bool>(&anomaly_detection)->default_value(false), "Check for anomalies in logger.")
            ("normal-login-word", po::value<string>(&normal_login_word)->default_value("login"), "For anomaly detector- word that should symbol a login line in login/logout logger (after '+' sign).")
             ;

    pd.add("log-path", -1);

    visible_desc.add(generic_group).add(calender_group).add(log_group).add(anomalies_group);
    full_desc.add(generic_group).add(calender_group).add(log_group).add(anomalies_group).add(hidden_options_group);

    /// Command line options applying
    po::variables_map vm;
    po::store(
            po::command_line_parser(ac, av)
            .options(full_desc)
            .positional(pd)
            .run(), vm);
    po::notify(vm);

    /// --help / -h option handler
    if (vm.count("help")) {
        cout << visible_desc << "\n";
        return 1;
    }

    /// --log-path / -l option handler
    if (!boost::filesystem::exists(log_file_path))
        throw std::runtime_error("Log file path doesn't exist.");

    /// --week-start-day / -d option handler
    /// Initialize available days list
    auto available_days = vector<day>{{"sunday", boost::date_time::weekdays::Sunday},
                           {"monday", boost::date_time::weekdays::Monday},
                           {"tuesday", boost::date_time::weekdays::Tuesday},
                           {"wednesday", boost::date_time::weekdays::Wednesday},
                           {"thursday", boost::date_time::weekdays::Thursday},
                           {"friday", boost::date_time::weekdays::Friday},
                           {"saturday", boost::date_time::weekdays::Saturday}};
    if (auto selected_day = std::find(available_days.begin(), available_days.end(), boost::to_lower_copy(week_start_day)); selected_day != available_days.end()) { // Selected day exists
        log_handler::week_start_day = selected_day->day_symbol;
    } else { // Selected day doesn't exists
        if (week_start_day == "help") { // Produce help days message
            cout << "Available days:" << endl;
            cout << "\tSun [Sunday]" << endl;
            cout << "\tMon [Monday]" << endl;
            cout << "\tTue [Tuesday]" << endl;
            cout << "\tWed [Wednesday]" << endl;
            cout << "\tThu [Thursday]" << endl;
            cout << "\tFri [Friday]" << endl;
            cout << "\tSat [Saturday]" << endl;
            return 0;
        }
        throw std::runtime_error("Unfamiliar day, for options list use '-d [ --week-start-day ] help'.");
    }

    // Anomalies detector
    auto anomaly_detected = log_handler::anomalies_detector(log_file_path, normal_login_word, anomaly_detection);
    if (analyze_activity) // Analyze logger times
        log_handler::analyze(log_file_path, verbose);
    if (anomaly_detected) // Produce anomalies warning if needed
        cout << "\n\n" << design_text::make_colored(stringstream() << "*** Anomaly detected! ***", design_text::Color::NONE, design_text::Color::RED, true) << endl;

    return 0;
}