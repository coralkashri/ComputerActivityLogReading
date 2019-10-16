#ifndef COMPUTERMONITORINGSTATISTICSPARSER_ANOMALIES_DETECTOR_H
#define COMPUTERMONITORINGSTATISTICSPARSER_ANOMALIES_DETECTOR_H

#include <iostream>

class anomalies_detector {
public:

    explicit anomalies_detector(const std::string &log_path, bool verbose, const std::string &normal_login_word);

    void detect();

    bool is_error() const;

    void clear_error_flag();

    void set_log_path(const std::string &log_path);

    void set_verbose(bool verbose);

private:
    bool error_flag;
    bool verbose;
    std::string normal_login_word;
    std::string log_path;
};

#endif //COMPUTERMONITORINGSTATISTICSPARSER_ANOMALIES_DETECTOR_H