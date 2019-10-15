#ifndef COMPUTERMONITORINGSTATISTICSPARSER_CMD_OPTIONS_PROCESSOR_H
#define COMPUTERMONITORINGSTATISTICSPARSER_CMD_OPTIONS_PROCESSOR_H

#include <iostream>
#include <boost/date_time/date_defs.hpp>
#include "cmd_options_basic_data.h"
#include "cmd_options_progress_data.h"

class cmd_options_processor {
public:

    /**
     * Ctor
     *
     * @brief initialize _options_data
     */
    explicit cmd_options_processor(const cmd_options_basic_data &options_data);

    /**
     * @brief Call basic params handlers
     *
     * @throw HelpMessageProducedException
     * @throw FileNotFoundException
     * @throw std::runtime_error - if unexpected error occurred
     */
    [[nodiscard]] cmd_options_progress_data process_basic_cmd_data() const;

private:

    /**
     * @brief --help / -h option handler
     *
     * @throw HelpMessageProducedException - if help flag was on
     */
    void help_message_handler() const;

    /**
     * @brief --log-path / -l option handler
     *
     * @throw FileNotFoundException - if logger file not found
     */
    void logger_path_handler() const;

    /**
     * @brief --week-start-day / -d option handler
     *
     * @throw HelpMessageProducedException - if day not found.
     * @throw std::runtime_error - if unexpected error occurred during the day search.
     *
     * @return week_start_day member value of cmd_options_progress_data
     */
    [[nodiscard]] boost::date_time::weekdays week_start_day_handler() const;

    cmd_options_basic_data _options_data;
};


#endif //COMPUTERMONITORINGSTATISTICSPARSER_CMD_OPTIONS_PROCESSOR_H