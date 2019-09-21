#include "design_text.h"

namespace design_text {
    inline std::string BoolToString(bool b) {
        return b ? "1" : "10";
    }

    std::string make_colored(const std::string &text, Color fgcolor, Color bgcolor, bool bold) {
        std::string new_text;
        new_text = "\033[";
        switch (fgcolor) {
            case Color::BLACK:
                new_text += ";30m";
                break;
            case Color::RED:
                new_text += ";31m";
                break;
            case Color::GREEN:
                new_text += /*";92m"*/";38;2;0;170;0m";
                break;
            case Color::YELLOW:
                new_text += ";33m";
                break;
            case Color::BLUE:
                new_text += ";34m";
                break;
            case Color::MAGENTA:
                new_text += ";35m";
                break;
            case Color::CYAN:
                new_text += ";36m";
                break;
            case Color::WHITE:
                new_text += ";37m";
                break;
            case Color::NONE:
                new_text += ";38m";
                break;
        }
        new_text += "\033[";
        switch (bgcolor) {
            case Color::BLACK:
                new_text += BoolToString(bold) + ";40m" + text;
                break;
            case Color::RED:
                new_text += BoolToString(bold) + ";41m" + text;
                break;
            case Color::GREEN:
                new_text += BoolToString(bold) + /*";102m"*/";48;2;0;170;0m" + text;
                break;
            case Color::YELLOW:
                new_text += BoolToString(bold) + ";43m" + text;
                break;
            case Color::BLUE:
                new_text += BoolToString(bold) + ";44m" + text;
                break;
            case Color::MAGENTA:
                new_text += BoolToString(bold) + ";45m" + text;
                break;
            case Color::CYAN:
                new_text += BoolToString(bold) + ";46m" + text;
                break;
            case Color::WHITE:
                new_text += BoolToString(bold) + ";47m" + text;
                break;
            case Color::NONE:
                new_text += BoolToString(bold) + ";48m" + text;
                break;
        }
        new_text += "\033[0m";
        return new_text;
    }

    std::string make_colored(const std::stringstream &text, Color fgcolor, Color bgcolor, bool bold) {
        return make_colored(text.str(), fgcolor, bgcolor, bold);
    }

    std::string make_colored(std::basic_ostream<char> &text, Color fgcolor, Color bgcolor, bool bold) {
        std::stringstream s;
        s << text.rdbuf();
        return make_colored(s.str(), fgcolor, bgcolor, bold);
    }
}