/*
* Created by suemi on 2017/2/27.
*/

#ifndef SLOG_OBJECT_FACTORY_H
#define SLOG_OBJECT_FACTORY_H

#include <string>
#include <unordered_map>
#include <functional>
#include <memory>

#include "slog/logging/log_guard.h"

namespace slog {

class Properties;

class Appender;

class Filter;


class Layout;


class ErrorHandler;


template<class T,typename... Args>
class ObjectFactory{
  using self = ObjectFactory<T,Args...>;
  using Ptr = std::shared_ptr<T>;
  using ConstructFunc = std::function<Ptr(const Args&...)>;
public:

  virtual ~ObjectFactory(){}

  static void Regist(const std::string& clazz, const ConstructFunc& func){
    self* ptr = Impl();
    ptr->constructors_[clazz] = func;
  }

  static Ptr CreateInstance(const std::string& clazz, const Args&... args){
    self* ptr = Impl();
    auto it = ptr->constructors_.find(clazz);
    if(it==ptr->constructors_.end())
      LogGuard::Instance()->Error("Class Not Found: "+clazz+", Please config a valid type!", true);
    return it->second(args...);
  }

  static self* Impl(){
    if(instance_== nullptr) instance_ = new ObjectFactory();
    return instance_;
  }

protected:

  static self* instance_;

  std::unordered_map<std::string,ConstructFunc> constructors_;
};

template<class T,typename... Args>
ObjectFactory<T,Args...>* ObjectFactory<T,Args...>::instance_ = nullptr;

template<>
ObjectFactory<Appender, Properties> *ObjectFactory<Appender, Properties>::Impl();

template<>
ObjectFactory<ErrorHandler> *ObjectFactory<ErrorHandler>::Impl();

template<>
ObjectFactory<Filter, Properties> *ObjectFactory<Filter, Properties>::Impl();

template<>
ObjectFactory<Layout,Properties>* ObjectFactory<Layout,Properties>::Impl();



}

#endif 
