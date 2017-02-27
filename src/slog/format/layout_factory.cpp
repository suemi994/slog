/*
* Created by suemi on 2017/2/27.
*/

#include "slog/format/simple_layout.h"
#include "slog/format/pattern_layout.h"
#include "slog/format/layout_factory.h"

namespace slog {

template<>
ObjectFactory<Layout,Properties>::self* ObjectFactory<Layout,Properties>::Impl(){
  if(instance_== nullptr) instance_ = new LayoutFactory();
  return instance_;
};

LayoutFactory::LayoutFactory() {
  constructors_.insert({"SimpleLayout",[](const Properties& prop){
    return std::make_shared<SimpleLayout>(prop);
  }});

  constructors_.insert({"PatternLayout",[](const Properties& prop){
    return std::make_shared<PatternLayout>(prop);
  }});
}

}