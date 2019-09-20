cmake_minimum_required(VERSION 3.12)
project(ComputerMonitoringStatisticsParser)

set(BOOST_REQUIRED_COMPONENTS
        system
        thread
        regex
        coroutine
        context
        filesystem
        date_time
        unit_test_framework
        program_options
        )
find_package(Boost "1.54.0" REQUIRED ${BOOST_REQUIRED_COMPONENTS})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*")

add_executable(ComputerMonitoringStatisticsParser ${SOURCES} src/utilities/design_text.cpp src/utilities/design_text.h src/TimeContainer.cpp src/TimeContainer.h src/utilities/boost_extentions.cpp src/utilities/boost_extentions.h src/utilities/std_extentions.cpp src/utilities/std_extentions.h)

target_link_libraries(ComputerMonitoringStatisticsParser ${Boost_LIBRARIES})