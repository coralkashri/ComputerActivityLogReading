#ifndef COMPUTERMONITORINGSTATISTICSPARSER_CMD_OPTIONS_H
#define COMPUTERMONITORINGSTATISTICSPARSER_CMD_OPTIONS_H

#include <iostream>
#include <boost/program_options.hpp>
#include "cmd_options_basic_data.h"

class cmd_options_parser {
public:
    explicit cmd_options_parser(int ac, char* av[]);

    cmd_options_basic_data get_data();

    void process_data();

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

    cmd_options_basic_data _options_data;
    boost::program_options::options_description full_options;
    boost::program_options::positional_options_description positional_options;
};

template<class... Args>
boost::program_options::options_description cmd_options_parser::group_cmd_options(const boost::program_options::options_description &option, Args&... options) {
    boost::program_options::options_description group;
    group.add(option);
    group.add(group_cmd_options(options...));
    return group;
}

#endif //COMPUTERMONITORINGSTATISTICSPARSER_CMD_OPTIONS_H