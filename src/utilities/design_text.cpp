//
// Created by sherlock on 9/14/19.
//

#include "design_text.h"

namespace design_text {
    inline std::string BoolToString(bool b) {
        return b ? "1" : "0";
    }

    std::string make_colored(const std::string &text, Color color, bool bold) {
        std::string new_text;
        new_text = "\033[";
        switch (color) {
            case Color::BLACK:
                new_text += BoolToString(bold) + ";30m" + text;
                break;
            case Color::RED:
                new_text += BoolToString(bold) + ";31m" + text;
                break;
            case Color::GREEN:
                new_text += BoolToString(bold) + ";32m" + text;
                break;
            case Color::YELLOW:
                new_text += BoolToString(bold) + ";33m" + text;
                break;
            case Color::BLUE:
                new_text += BoolToString(bold) + ";34m" + text;
                break;
            case Color::MAGENTA:
                new_text += BoolToString(bold) + ";35m" + text;
                break;
            case Color::CYAN:
                new_text += BoolToString(bold) + ";36m" + text;
                break;
            case Color::WHITE:
                new_text += BoolToString(bold) + ";37m" + text;
                break;
        }
        new_text += "\033[0m";
        return new_text;
    }
}