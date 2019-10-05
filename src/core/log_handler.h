#ifndef COMPUTERMONITORINGSTATISTICSPARSER_LOG_HANDLER_H
#define COMPUTERMONITORINGSTATISTICSPARSER_LOG_HANDLER_H

#include <iostream>

namespace log_handler {
    void analyze(std::string log_path, bool verbose = false);
    bool anomalies_detector(std::string log_path);
};


#endif //COMPUTERMONITORINGSTATISTICSPARSER_LOG_HANDLER_H
