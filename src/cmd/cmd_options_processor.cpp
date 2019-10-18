#include "cmd_options_processor.h"
#include "../utilities/exceptions.h"
#include "../useful_collections/week.h"
#include "../core/log_handler.h"
#include <boost/filesystem.hpp>

cmd_options_processor::cmd_options_processor(const cmd_options_basic_data &options_data)
        : _options_data(options_data) {
}

cmd_options_progress_data cmd_options_processor::process_basic_cmd_data() const {
    try {
        cmd_options_progress_data extracted_data;
        help_message_handler();
        logger_path_handler();
        extracted_data.week_start_day = week_start_day_handler();
        return extracted_data;
    } catch (...) {
        throw;
    }
}

void cmd_options_processor::help_message_handler() const {
    if (_options_data.help) {
        std::cout << _options_data.visible_options << "\n";
        throw exceptions::HelpMessageProducedException();
    }
}

void cmd_options_processor::logger_path_handler() const {
    if (!boost::filesystem::exists(_options_data.log_file_path)) {
        throw exceptions::FileNotFoundException();
    }
}

boost::date_time::weekdays cmd_options_processor::week_start_day_handler() const {
    try {
        week w;
        return w.get_day(_options_data.week_start_day).day_symbol;
    } catch (exceptions::HelpMessageProducedException &hd_e) {
        throw;
    } catch (exceptions::DayNotFoundException &dnf_e) {
        std::cout << "Unfamiliar day, for options list use '-d [ --week-start-day ] help'." << std::endl;
        throw exceptions::HelpMessageProducedException();
    } catch (std::exception &e) {
        throw std::runtime_error("Unexpected error occurred during day searching.");
    }
}