#ifndef COMPUTERMONITORINGSTATISTICSPARSER_EXCEPTIONS_H
#define COMPUTERMONITORINGSTATISTICSPARSER_EXCEPTIONS_H

#include <iostream>

namespace exceptions {
    struct DayNotFoundException : public std::exception
    {
        const char* what() const throw();
    };

    struct FileNotFoundException : public std::exception
    {
        const char* what() const throw();
    };

    struct HelpMessageProducedException : public std::exception
    {
        const char* what() const throw();
    };
};


#endif //COMPUTERMONITORINGSTATISTICSPARSER_EXCEPTIONS_H
