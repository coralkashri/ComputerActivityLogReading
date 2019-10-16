#include "anomalies_detector.h"
#include <fstream>
#include "../utilities/design_text.h"

using namespace design_text;

anomalies_detector::anomalies_detector(const std::string &log_path, bool verbose, const std::string &normal_login_word)
        : log_path(log_path), verbose(verbose), normal_login_word(normal_login_word), error_flag(false) {}

void anomalies_detector::detect() {
    bool anomaly_detected = false;
    std::ostream temp(nullptr); // Don't produce anomalies details message
    std::ostream &anomalies_output = verbose ? std::cout : temp;
    if (error_flag) {
        std::cerr << "WARNING: anomalies_detector::detect() get called, without clearing error flag before." << std::endl;
    }

    std::ifstream log_file(log_path, std::ios::in);
    std::string datetime, description_part;
    std::string prev_line;
    bool is_login_line_turn = true;
    size_t line_number = 0;
    while (std::getline(log_file, datetime, '+')) {
        line_number++;
        std::getline(log_file, description_part, '\n');
        if (auto place_of_login_word = description_part.find(normal_login_word);
                (is_login_line_turn && place_of_login_word == std::string::npos) ||
                (!is_login_line_turn && place_of_login_word != std::string::npos)) {

            if (!anomaly_detected) { // First anomaly - print anomalies title
                anomalies_output << "Anomalies:" << std::endl;
            }

            anomalies_output << make_colored(std::stringstream() << "Line " << (line_number - 1) << ": " << prev_line, Color::GREEN) << "\n";
            anomalies_output << make_colored(std::stringstream() << "Line " << line_number << ": " << datetime << "+" << description_part, Color::RED, Color::NONE, true) << "\n" << std::endl;
            anomaly_detected = true;
            is_login_line_turn = !is_login_line_turn;
        }
        is_login_line_turn = !is_login_line_turn;
        prev_line = datetime + "+" + description_part;
    }
    error_flag = error_flag || anomaly_detected;
}

bool anomalies_detector::is_error() const {
    return error_flag;
}

void anomalies_detector::clear_error_flag() {
    error_flag = false;
}

void anomalies_detector::set_log_path(const std::string &log_path) {
    this->log_path = log_path;
}

void anomalies_detector::set_verbose(bool verbose) {
    this->verbose = verbose;
}