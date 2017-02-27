/*
* Created by suemi on 2017/2/27.
*/

#ifndef SLOG_FILTER_FACTORY_H
#define SLOG_FILTER_FACTORY_H

#include "slog/base/object_factory.h"

namespace slog {

class Filter;

class Properties;

class FilterFactory : public ObjectFactory<Filter,Properties> {
public:
  FilterFactory();
};

}

#endif 
