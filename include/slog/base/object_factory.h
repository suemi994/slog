/*
* Created by suemi on 2017/2/27.
*/

#ifndef SLOG_OBJECT_FACTORY_H
#define SLOG_OBJECT_FACTORY_H

#include <string>
#include <unordered_map>
#include <functional>
#include <memory>

namespace slog {

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
    if(it==ptr->constructors_.end()) return nullptr;
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

}

#endif 
