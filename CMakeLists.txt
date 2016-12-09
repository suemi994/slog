cmake_minimum_required(VERSION 3.6)
project(slog)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)
include_directories(include)

set(SOURCE_FILES main.cpp include/slog/logging/logger.h include/slog/utils/no_copyable.h include/slog/logging/logger_factory.h include/slog/base/configuration.h include/slog/base/fixed_buffer.h include/slog/appender/appender.h include/slog/appender/file_appender.h include/slog/appender/console_appender.h include/slog/utils/properties.h include/slog/format/layout.h include/slog/format/simple_layout.h include/slog/format/pattern_layout.h include/slog/format/formatter.h include/slog/filter/filter.h include/slog/filter/filter_builder.h include/slog/logging/log_scheduler.h include/slog/logging/log_event.h include/slog/logging/log_level.h include/slog/appender/null_appender.h include/slog/appender/error_handler.h include/slog/logging/log_macros.h src/slog/base/fixed_buffer.cpp)

find_package(GTest 1.80 REQUIRED)
if (GTEST_FOUND)
    include_directories(${GTEST_INCLUDE_DIRS})
endif (GTEST_FOUND)

add_executable(slog ${SOURCE_FILES})