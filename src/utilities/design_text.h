#ifndef COMPUTERMONITORINGSTATISTICSPARSER_DESIGN_TEXT_H
#define COMPUTERMONITORINGSTATISTICSPARSER_DESIGN_TEXT_H

#include <string>
#include <sstream>

namespace design_text {
    enum class Color {
        BLACK,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        WHITE,
        NONE
    };

    std::string make_colored(const std::string &text, Color fgcolor, Color bgcolor = Color::NONE, bool bold = false);
    std::string make_colored(const std::stringstream &text, Color fgcolor, Color bgcolor = Color::NONE, bool bold = false);
    std::string make_colored(std::basic_ostream<char> &text, Color fgcolor, Color bgcolor = Color::NONE, bool bold = false);
}

#endif //COMPUTERMONITORINGSTATISTICSPARSER_DESIGN_TEXT_H
