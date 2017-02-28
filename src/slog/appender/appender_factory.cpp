/*
* Created by suemi on 2017/2/28.
*/

#include "slog/appender/all.h"


namespace slog {

template<>
ObjectFactory<Appender, Properties>::self *ObjectFactory<Appender, Properties>::Impl() {
  if (instance_ == nullptr) instance_ = new AppenderFactory();
  return instance_;
};

AppenderFactory::AppenderFactory() {
  constructors_.insert({"ConsoleAppender", [](const Properties &prop) {
    return std::make_shared<ConsoleAppender>(prop);
  }});

  constructors_.insert({"NullAppender", [](const Properties &prop) {
    return std::make_shared<NullAppender>(prop);
  }});

  constructors_.insert({"FileAppender", [](const Properties &prop) {
    return std::make_shared<FileAppender>(prop);
  }});

  constructors_.insert({"CapacityRollingFileAppender", [](const Properties &prop) {
    return std::make_shared<CapacityRollingFileAppender>(prop);
  }});

  constructors_.insert({"CycleRollingFileAppender", [](const Properties &prop) {
    return std::make_shared<CycleRollingFileAppender>(prop);
  }});
}

template<>
ObjectFactory<ErrorHandler>::self *ObjectFactory<ErrorHandler>::Impl() {
  if (instance_ == nullptr) instance_ = new ErrorHandlerFactory();
  return instance_;
};

ErrorHandlerFactory::ErrorHandlerFactory() {
  constructors_.insert({"IgnoreHandler", []() {
    return std::make_shared<IgnoreHandler>();
  }});

  constructors_.insert({"AbortHandler", []() {
    return std::make_shared<AbortHandler>();
  }});

  constructors_.insert({"RetryHandler", []() {
    return std::make_shared<RetryHandler>();
  }});
}

}