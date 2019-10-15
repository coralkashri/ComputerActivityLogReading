#include "cmd_options.h"

namespace boost_cmd_po = boost::program_options;

cmd_options::cmd_options(int ac, char* av[]) : _options_data("Usage: program [options] [path/]logger_filename") {

    auto generic_options = init_cmd_po_generic_options();
    auto calender_options = init_cmd_po_calender_options();
    auto logger_options = init_cmd_po_logger_options();
    auto mode_options = init_cmd_po_mode_options();
    auto hidden_options = init_cmd_po_hidden_options();

    _options_data.visible_options.add(
            group_cmd_options(
                    generic_options,
                    calender_options,
                    logger_options,
                    mode_options
            )
    );

    full_options.add(
            group_cmd_options(
                    generic_options,
                    calender_options,
                    logger_options,
                    mode_options,
                    hidden_options
            )
    );
    positional_options = init_cmd_positional_options();

    apply_program_options(ac, av);

    update_flags();
}

boost_cmd_po::options_description cmd_options::init_cmd_po_generic_options() {
    auto group = boost_cmd_po::options_description("Generic options");
    group.add_options()
            ("help,h", "produce help message")
            //("verbose", boost_cmd_po::value<bool>(&_options_data.verbose)->default_value(false), "Show detailed times of login.");
            ("verbose", "Show detailed times of login.");
    return group;
}

boost_cmd_po::options_description cmd_options::init_cmd_po_calender_options() {
    auto group = boost_cmd_po::options_description("Calender options");
    group.add_options()
            ("week-start-day,d", boost_cmd_po::value<std::string>(&_options_data.week_start_day)->default_value("Monday"), "Week starting day ('--week-start-day help' for a list).");
    return group;
}

boost_cmd_po::options_description cmd_options::init_cmd_po_logger_options() {
    auto group = boost_cmd_po::options_description("Logger options");
    group.add_options();
    return group;
}

boost_cmd_po::options_description cmd_options::init_cmd_po_hidden_options() {
    auto group = boost_cmd_po::options_description("Logger options");
    group.add_options()
            ("log-path,l", boost_cmd_po::value<std::string>(&_options_data.log_file_path)->default_value( "/home/sherlock/message_from_computer"), "Path to login/logout logger.");
    return group;
}

boost_cmd_po::options_description cmd_options::init_cmd_po_mode_options() {
    auto group = boost_cmd_po::options_description("Mode options");
    group.add_options()
            //("analyze-log", boost_cmd_po::value<bool>(&_options_data.analyze_activity)->default_value(true), "Analyze activity - show activity times and summarise activity.")
            ("no-analyze", "Disable activity analyzing - don't show activity times/summarise.")
            //("anomaly-detection", boost_cmd_po::value<bool>(&_options_data.anomaly_detection)->default_value(false), "Check for anomalies in logger.")
            ("anomaly-detection", "Check for anomalies in logger.")
            ("normal-login-word", boost_cmd_po::value<std::string>(&_options_data.normal_login_word)->default_value("login"), "For anomaly detector- word that should symbol a login line in login/logout logger (after '+' sign).");
    return group;
}

boost_cmd_po::positional_options_description cmd_options::init_cmd_positional_options() {
    boost_cmd_po::positional_options_description pd;
    pd.add("log-path", -1);
    return pd;
}

void cmd_options::apply_program_options(int ac, char **av) {
    boost_cmd_po::store(
            boost_cmd_po::command_line_parser(ac, av)
                    .options(full_options)
                    .positional(positional_options)
                    .run(), _options_data.variables_map);
    boost_cmd_po::notify(_options_data.variables_map);
}

void cmd_options::update_flags() {
    _options_data.help              = (bool) _options_data.variables_map.count("help");
    _options_data.verbose           = (bool) _options_data.variables_map.count("verbose");
    _options_data.analyze_activity  = !(bool) _options_data.variables_map.count("no-analyze");
    _options_data.anomaly_detection = (bool) _options_data.variables_map.count("anomaly-detection");
}

cmd_options_data cmd_options::get_data() {
    return _options_data;
}