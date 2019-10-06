#include <iostream>
#include <boost/program_options.hpp>
#include "core/log_handler.h"

using namespace std;
namespace po = boost::program_options;

int main(int ac, char* av[]) {
    po::options_description desc("Allowed options");
    bool verbose, anomaly_detection;
    string normal_login_word;
    string log_file_path;
    desc.add_options()
            ("help", "produce help message")
            ("verbose", po::value<bool>(&verbose)->default_value(true), "Show detailed times of login.")
            ;
    auto log_group = po::options_description("Logger Options");
    log_group.add_options()
            ("log-path", po::value<string>(&log_file_path)->default_value("/home/sherlock/message_from_computer_DEBUG"), "Path to login/logout logger.")
            ;
    auto anomalies_group = po::options_description("Anomalies Detector Options");
    anomalies_group.add_options()
            ("anomaly-detection", po::value<bool>(&anomaly_detection)->default_value(false), "Check for anomalies in log.")
            ("normal-login-word", po::value<string>(&normal_login_word)->default_value("login"), "For anomaly detector- word that should symbol a login line in login/logout logger (after '+' sign).")
             ;

    desc.add(log_group);
    desc.add(anomalies_group);

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    if (!anomaly_detection) {
        log_handler::analyze(log_file_path, verbose);
    } else {
        auto anomaly_detected = log_handler::anomalies_detector(log_file_path);
    }

    return 0;
}