#include "exceptions.h"

namespace exceptions {

    const char* DayNotFoundException::what() const throw() {
        return "Selected day not found.";
    }

    const char* FileNotFoundException::what() const throw() {
        return "File not found.";
    }

    const char* HelpMessageProducedException::what() const throw() {
        return "An help message have been produced.";
    }

}