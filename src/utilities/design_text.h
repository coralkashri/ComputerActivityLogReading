//
// Created by sherlock on 9/14/19.
//

#ifndef COMPUTERMONITORINGSTATISTICSPARSER_DESIGN_TEXT_H
#define COMPUTERMONITORINGSTATISTICSPARSER_DESIGN_TEXT_H

#include <string>

namespace design_text {
    enum class Color {
        BLACK,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        WHITE
    };

    std::string make_colored(const std::string &text, Color color, bool bold);
}

#endif //COMPUTERMONITORINGSTATISTICSPARSER_DESIGN_TEXT_H
