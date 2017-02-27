/*
* Created by suemi on 2017/2/27.
*/

#ifndef SLOG_LAYOUT_FACTORY_H
#define SLOG_LAYOUT_FACTORY_H

#include "slog/base/object_factory.h"

namespace slog {

class Layout;

class Properties;

class LayoutFactory : public ObjectFactory<Layout,Properties> {
public:
  LayoutFactory();
};

}

#endif 
