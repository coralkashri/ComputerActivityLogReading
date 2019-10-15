#ifndef COMPUTERMONITORINGSTATISTICSPARSER_CMD_OPTIONS_H
#define COMPUTERMONITORINGSTATISTICSPARSER_CMD_OPTIONS_H

#include <iostream>
#include <boost/program_options.hpp>

struct cmd_options_data {
    explicit cmd_options_data(const std::string &options_description) :
            visible_options(options_description) {}

    bool help = false;                      // Show help message
    bool verbose = false;                   // Display login/logout details
    bool anomaly_detection = false;         // Show anomalies details if found
    bool analyze_activity = true;           // Analyze login/logout total/summarize times
    std::string week_start_day;
    std::string log_file_path;
    std::string normal_login_word;
    boost::program_options::options_description visible_options;
    boost::program_options::variables_map variables_map;
};

class cmd_options {
public:
    explicit cmd_options(int ac, char* av[]);

    cmd_options_data get_data();

private:
    boost::program_options::options_description init_cmd_po_generic_options();

    boost::program_options::options_description init_cmd_po_calender_options();

    boost::program_options::options_description init_cmd_po_logger_options();

    boost::program_options::options_description init_cmd_po_hidden_options();

    boost::program_options::options_description init_cmd_po_mode_options();

    boost::program_options::positional_options_description init_cmd_positional_options();

    boost::program_options::options_description group_cmd_options() {
        return boost::program_options::options_description();
    }

    template<class... Args>
    boost::program_options::options_description group_cmd_options(const boost::program_options::options_description &option, Args&... options);

    void apply_program_options(int ac, char* av[]);

    void update_flags();

    cmd_options_data _options_data;
    boost::program_options::options_description full_options;
    boost::program_options::positional_options_description positional_options;
};

template<class... Args>
boost::program_options::options_description cmd_options::group_cmd_options(const boost::program_options::options_description &option, Args&... options) {
    boost::program_options::options_description group;
    group.add(option);
    group.add(group_cmd_options(options...));
    return group;
}

#endif //COMPUTERMONITORINGSTATISTICSPARSER_CMD_OPTIONS_H