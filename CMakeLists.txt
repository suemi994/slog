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
        include/slog/base/configurator.h
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
        include/slog/utils/time_util.h
        test/slog/logging/test_log_event.cpp
        src/slog/logging/logger_factory.cpp
        src/slog/logging/log_scheduler.cpp
        src/slog/appender/error_handler.cpp
        src/slog/filter/filter.cpp
        src/slog/filter/filter_builder.cpp
        src/slog/appender/appender.cpp
        src/slog/appender/console_appender.cpp
        src/slog/appender/null_appender.cpp
        include/slog/appender/custom_appender.h
        src/slog/appender/custom_appender.cpp
        include/slog/utils/file_util.h
        include/slog/utils/string_util.h
        src/slog/utils/string_util.cpp
        src/slog/utils/file_util.cpp
        src/slog/utils/time_util.cpp
        include/slog/logging/log_guard.h
        src/slog/logging/log_guard.cpp
        src/slog/appender/file_appender.cpp
        src/slog/format/layout.cpp
        src/slog/format/simple_layout.cpp
        src/slog/format/pattern_layout.cpp
        include/slog/utils/count_down_latch.h
        src/slog/utils/count_down_latch.cpp
        src/slog/logging/log_macros.cpp
        src/slog/logging/log_level.cpp
        src/slog/base/configurator.cpp
        include/slog/base/object_factory.h
        include/slog/appender/appender_factory.h
        include/slog/format/layout_factory.h
        include/slog/filter/filter_factory.h
        src/slog/format/layout_factory.cpp
        src/slog/filter/filter_factory.cpp
        src/slog/appender/appender_factory.cpp
        include/slog/appender/all.h test/slog/utils/test_string_util.cpp test/slog/utils/test_file_util.cpp test/slog/utils/time_util_main.cpp test/slog/logging/logging_guard_main.cpp test/slog/format/test_layout.cpp test/slog/logging/test_log_level.cpp)

set(TEST_FILES
        src/slog/utils/formatter.cpp
        test/slog/utils/test_formatter.cpp

        src/slog/utils/string_util.cpp
        test/slog/utils/test_string_util.cpp

        src/slog/utils/properties.cpp
        test/slog/utils/test_properties.cpp

        src/slog/logging/log_level.cpp
        test/slog/logging/test_log_level.cpp
        )

SET(TMP_FILES
        src/slog/logging/log_guard.cpp
        main.cpp
        )
add_executable(tmp_main ${TMP_FILES})

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

