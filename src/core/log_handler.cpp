#include "log_handler.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include "../utilities/std_extentions.h"
#include "../utilities/boost_extentions.h"
#include "../utilities/design_text.h"

using namespace design_text;

log_handler::log_handler(bool analyze_verbose,
                         bool enable_analyze,
                         const std::string &log_path,
                         bool anomalies_verbose,
                         const std::string &normal_login_word)
        : enable_analyze(enable_analyze),
          analyzer(analyze_verbose, log_path),
          detector(log_path, anomalies_verbose, normal_login_word) {
}

void log_handler::config_analyzer_params(boost::date_time::weekdays week_start_day, float sleep_hours_per_day, float study_day_hours_in_week) {
    analyzer.config_analyze_params(week_start_day, sleep_hours_per_day, study_day_hours_in_week);
}

void log_handler::process() {
    // Anomalies detector
    detector.detect();
    if (enable_analyze) // Analyze logger times
        analyzer.analyze();
    if (detector.is_error()) // Produce anomalies warning if needed
        std::cout << "\n\n" << design_text::make_colored(std::stringstream() << "*** Anomaly detected! ***", design_text::Color::NONE, design_text::Color::RED, true) << std::endl;
}