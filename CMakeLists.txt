cmake_minimum_required(VERSION 3.6)
project(slog)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)
include_directories(include)

set(SOURCE_FILES
        main.cpp
        include/slog/logging/logger.h
        include/slog/utils/no_copyable.h
        include/slog/logging/logger_factory.h
        include/slog/base/configuration.h
        include/slog/base/fixed_buffer.h
        include/slog/appender/appender.h
        include/slog/appender/file_appender.h
        include/slog/appender/console_appender.h
        include/slog/utils/properties.h
        include/slog/format/layout.h
        include/slog/format/simple_layout.h
        include/slog/format/pattern_layout.h
        include/slog/utils/formatter.h
        include/slog/filter/filter.h
        include/slog/filter/filter_builder.h
        include/slog/logging/log_scheduler.h
        include/slog/logging/log_event.h
        include/slog/logging/log_level.h
        include/slog/appender/null_appender.h
        include/slog/appender/error_handler.h
        include/slog/logging/log_macros.h
        src/slog/base/fixed_buffer.cpp
        src/slog/utils/formatter.cpp
        test/slog/utils/test_formatter.cpp
        src/slog/utils/properties.cpp
        test/slog/utils/test_properties.cpp
        src/slog/logging/logger.cpp
        src/slog/logging/log_event.cpp
        include/slog/utils/time_helper.h
        test/slog/logging/test_log_event.cpp
        src/slog/logging/logger_factory.cpp
        src/slog/logging/log_scheduler.cpp
        src/slog/appender/error_handler.cpp src/slog/filter/filter.cpp src/slog/filter/filter_builder.cpp)

set(TEST_FILES
        src/slog/utils/formatter.cpp
        test/slog/utils/test_formatter.cpp

        src/slog/utils/properties.cpp
        test/slog/utils/test_properties.cpp
        )

add_executable(slog ${SOURCE_FILES})

find_package(GTest 1.80 REQUIRED)
if (GTEST_FOUND)
    include_directories(${GTEST_INCLUDE_DIRS})
    add_executable(slog_test ${TEST_FILES})
    target_link_libraries(slog_test ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
endif (GTEST_FOUND)

find_package(Boost 1.62 REQUIRED)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(slog ${Boost_LIBRARIES})
endif (Boost_FOUND)

