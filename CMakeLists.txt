cmake_minimum_required(VERSION 3.6)
project(slog)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(src)
include_directories(include)

link_directories(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

set(SOURCE_FILES
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
        include/slog/utils/time_util.h
        include/slog/appender/custom_appender.h
        include/slog/utils/file_util.h
        include/slog/utils/string_util.h
        include/slog/logging/log_guard.h
        include/slog/utils/count_down_latch.h
        include/slog/base/object_factory.h
        include/slog/appender/appender_factory.h
        include/slog/format/layout_factory.h
        include/slog/filter/filter_factory.h



        src/slog/base/fixed_buffer.cpp

        src/slog/utils/formatter.cpp
        src/slog/utils/properties.cpp
        src/slog/utils/string_util.cpp
        src/slog/utils/file_util.cpp
        src/slog/utils/time_util.cpp
        src/slog/utils/count_down_latch.cpp

        src/slog/logging/log_level.cpp
        src/slog/logging/log_guard.cpp
        src/slog/logging/logger.cpp
        src/slog/logging/log_event.cpp
        src/slog/logging/log_scheduler.cpp
        src/slog/logging/logger_factory.cpp
        src/slog/logging/log_macros.cpp

        src/slog/filter/filter.cpp
        src/slog/filter/filter_builder.cpp

        src/slog/appender/error_handler.cpp
        src/slog/appender/appender.cpp
        src/slog/appender/null_appender.cpp
        src/slog/appender/console_appender.cpp
        src/slog/appender/custom_appender.cpp
        src/slog/appender/file_appender.cpp

        src/slog/format/layout.cpp
        src/slog/format/simple_layout.cpp
        src/slog/format/pattern_layout.cpp

        src/slog/base/configurator.cpp

        src/slog/filter/filter_factory.cpp
        src/slog/appender/appender_factory.cpp
        src/slog/format/layout_factory.cpp
        )
set(TEST_FILES
        test/slog/utils/test_formatter.cpp
        test/slog/utils/test_string_util.cpp
        test/slog/utils/test_properties.cpp
        test/slog/logging/test_log_level.cpp
        test/slog/logging/test_log_event.cpp
        test/slog/utils/test_file_util.cpp
        test/slog/filter/test_filter.cpp
#        test/slog/format/test_layout.cpp
        )

SET(TMP_FILES

        test_main.cpp
        )
add_executable(test_main ${TMP_FILES})

add_dependencies(test_main slog)


target_link_libraries(test_main slog)

add_library(slog STATIC ${SOURCE_FILES})

INSTALL(TARGETS slog ARCHIVE DESTINATION /usr/local/lib)

find_package(GTest 1.80 REQUIRED)
if (GTEST_FOUND)
    include_directories(${GTEST_INCLUDE_DIRS})
    add_executable(slog_test ${TEST_FILES})
    add_dependencies(slog_test slog)
    target_link_libraries(slog_test ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread slog)
endif (GTEST_FOUND)





