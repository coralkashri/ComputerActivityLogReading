#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>
#include "useful_collections/week.h"
#include "core/log_handler.h"
#include "utilities/design_text.h"
#include "cmd/cmd_options_parser.h"
#include "cmd/cmd_options_processor.h"
#include "utilities/exceptions.h"

int main(int ac, char* av[]) {
    cmd_options_parser command_line_options(ac, av);
    cmd_options_basic_data cmd_basic_data = command_line_options.get_data();
    cmd_options_processor cmd_processor(cmd_basic_data);
    cmd_options_progress_data cmd_progress_data;

    /// Process basic cmd params
    try {
        cmd_progress_data = cmd_processor.process_basic_cmd_data();
    } catch (exceptions::HelpMessageProducedException &) {
        return EXIT_SUCCESS;
    } catch (exceptions::FileNotFoundException &file_not_found_e) {
        std::cerr << file_not_found_e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    /// Log handler initialize
    log_handler log_analyzer(
            cmd_basic_data.analyze_verbose,
            cmd_basic_data.analyze_activity,
            cmd_basic_data.log_file_path,
            cmd_basic_data.anomaly_detection,
            cmd_basic_data.normal_login_word
    );
    log_analyzer.config_analyzer_params(cmd_progress_data.week_start_day, cmd_basic_data.sleep_hours_per_day, cmd_basic_data.study_hours_in_week);

    /// Log handler process
    log_analyzer.process();

    return EXIT_SUCCESS;
}