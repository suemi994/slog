/*
* Created by suemi on 2017/2/27.
*/

#ifndef SLOG_APPENDER_FACTORY_H
#define SLOG_APPENDER_FACTORY_H

#include "slog/base/object_factory.h"

namespace slog {

class Appender;

class Properties;

class AppenderFactory : public ObjectFactory<Appender,Properties> {
public:
  AppenderFactory();
};

}

#endif 
