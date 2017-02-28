/*
* Created by suemi on 2017/2/28.
*/

#include "slog/filter/filter_factory.h"
#include "slog/filter/filter.h"

namespace slog {

template<>
ObjectFactory<FilterFactory,Properties>::self* ObjectFactory<FilterFactory,Properties>::Impl() {
  if(instance_== nullptr) instance_ = new FilterFactory();
  return instance_;
};

FilterFactory::FilterFactory() {
  constructors_.insert({"AcceptAllFilter",[](const Properties& prop){
    return std::make_shared<AcceptAllFilter>(prop);
  }});

  constructors_.insert({"DenyAllFilter",[](const Properties& prop){
    return std::make_shared<DenyAllFilter>(prop);
  }});

  constructors_.insert({"LevelMatchFilter",[](const Properties& prop){
    return std::make_shared<LogLevelMatchFilter>(prop);
  }});

  constructors_.insert({"LevelRangeFilter",[](const Properties& prop){
    return std::make_shared<LogLevelRangeFilter>(prop);
  }});

  constructors_.insert({"AndFilter",[](const Properties& prop){
    return std::make_shared<AndFilter>(prop);
  }});

  constructors_.insert({"BinaryAndFilter",[](const Properties& prop){
    return std::make_shared<BinaryAndFilter>(prop);
  }});

  constructors_.insert({"OrFilter",[](const Properties& prop){
    return std::make_shared<OrFilter>(prop);
  }});

  constructors_.insert({"BinaryOrFilter",[](const Properties& prop){
    return std::make_shared<BinaryOrFilter>(prop);
  }});

  constructors_.insert({"NotFilter",[](const Properties& prop){
    return std::make_shared<NotFilter>(prop);
  }});
}

}

