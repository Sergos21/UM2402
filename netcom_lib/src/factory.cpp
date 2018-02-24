/*! ***************************************************************************
* Проект: src.pro
*
* @file factory.cpp
*
* @brief: Файл исходных данных фабрики контрактов
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#include "factory.h"

using namespace netcom;
//--------------------------------------------------------------------------------------
template <typename ...Args>
bool BaseFactory<Args...>::registerContract(const QString &name,
                                            const typename BaseFactory<Args...>::Ctor &ctor,
                                            Args&&... args) {
  Hash &hash = getHash();
  if (hash.contains(name)) {
    const QScopedPointer<Contract> dummy { ctor(args)... };
    qWarning("Contract names must be unique. The current name '%s' is already used here '%s'",
             name.toLatin1().data(),
             dummy->metaObject()->className());
    return false;
  }
  else {
    hash[name] = ctor;
  }
  return true;
}
//--------------------------------------------------------------------------------------
template <typename ...Args>
Contract *BaseFactory<Args...>::createContractInstance(const QString &name, Args&&... args) {
  const Hash &hash = getHash();
  return (hash.contains(name) ?
          hash[name](std::forward<Args>(args)...) :
          Q_NULLPTR);
}
//--------------------------------------------------------------------------------------
template <typename ...Args>
typename BaseFactory<Args...>::Hash &BaseFactory<Args...>::getHash() {
  static Hash hash { };
  return hash;
}
//--------------------------------------------------------------------------------------

template class BaseFactory<>;
