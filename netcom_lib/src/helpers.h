/*! ***************************************************************************
* Проект: src.pro
*
* @file contract_impl.h
*
* @brief: Заголовочный файл вспомогательного классов, предназначенного для
* механизма авторегистрации контрактов
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#pragma once

#include "contract.h"
#include "factory.h"

namespace netcom {
namespace helpers {

// TODO: for c++14 more flexible implementation
/*template <char ...C>
struct string_literal {
  constexpr static const char value[sizeof...(C)] = { C... };
  constexpr operator const char*() { return value; }
};

template <char ...C> constexpr const char string_literal<C...>::value[sizeof...(C)];

template <typename T, char ...Cs>
constexpr string_literal<Cs...> operator "" _sl() {
  return { };
} */

/// Шаблон принудительной инициализации вызова соответствующего типа
template <typename T, T /*unnamed*/>
struct ForceInit { };

template <typename T, char ...Cs>
class BaseHelper : virtual public Contract {
  /// Проверка на наличие конструктора по умолчанию
  Q_STATIC_ASSERT_X(!std::is_default_constructible<T>::value,
                    "The contract class must have a default constructor");

  //Q_STATIC_ASSERT_X(!qtp::HasQ_OBJECT_Macro<T>::Value,
  //                  "The contract class must have macro Q_GADGET or Q_OBJECT");
public:
  BaseHelper() Q_DECL_EQ_DEFAULT;
  virtual ~BaseHelper() Q_DECL_EQ_DEFAULT;
  BaseHelper(const BaseHelper &other) Q_DECL_EQ_DELETE;
  BaseHelper(BaseHelper &&other) Q_DECL_EQ_DEFAULT;
  BaseHelper &operator=(const BaseHelper &other) Q_DECL_EQ_DELETE;
  BaseHelper &operator=(BaseHelper &&other) Q_DECL_EQ_DEFAULT;

  virtual const char *netcomName() const Q_DECL_OVERRIDE;
  virtual const QMetaObject *metaObject() const Q_DECL_OVERRIDE;

  static const char contract_name[sizeof...(Cs)] = { Cs... };

private:
  struct Proxy {
    explicit Proxy();
  };
  static Proxy _proxy_;
  using ProxyDummy = ForceInit<Proxy&, _proxy_>;
};

template <typename Base, typename Helper>
class DerivedHelper : public Base, public Helper {
public:
  DerivedHelper() Q_DECL_EQ_DEFAULT;
  virtual ~DerivedHelper() Q_DECL_EQ_DEFAULT;
  DerivedHelper(const DerivedHelper &other) Q_DECL_EQ_DELETE;
  DerivedHelper(DerivedHelper &&other) Q_DECL_EQ_DEFAULT;
  DerivedHelper &operator=(const DerivedHelper &other) Q_DECL_EQ_DELETE;
  DerivedHelper &operator=(DerivedHelper &&other) Q_DECL_EQ_DEFAULT;

  virtual const char *netcomName() const Q_DECL_OVERRIDE;
  virtual const QMetaObject *metaObject() const Q_DECL_OVERRIDE;
};

//--------------------ИНИЦИАЛИЗАЦИЯ СТАТИЧЕСКИХ ЧЛЕНОВ ШАБЛОНА--------------------------
template <typename T, char ...Cs>
typename BaseHelper<T, Cs...>::Proxy BaseHelper<T, Cs...>::_proxy_;
//--------------------------------------------------------------------------------------
template <typename T, char ...Cs>
Q_DECL_CONSTEXPR const char BaseHelper<T, Cs...>::contract_name[sizeof...(Cs)];
//----------------ОПИСАНИЕ ВСТРАИВАЕМЫХ МЕТОДОВ ШАБЛОНА BASE HELPER---------------------
/// Конструктор вспомогательного класса Proxy с регистрацией контракта
template <typename T, char ...Cs>
Q_OUTOFLINE_TEMPLATE BaseHelper<T, Cs...>::Proxy::Proxy() {
  registerContract<T>(contract_name);
}
//--------------------------------------------------------------------------------------
/// Получение имени контракта
template <typename T, char ...Cs>
Q_INLINE_TEMPLATE const char *BaseHelper<T, Cs...>::netcomName() const {
  return contract_name;
}
//--------------------------------------------------------------------------------------
/// Получение экземпляра класса QMetaObject
template <typename T, char ...Cs>
Q_INLINE_TEMPLATE const QMetaObject *BaseHelper<T, Cs...>::metaObject() const {
  return &T::staticMetaObject;
}
//--------------------------------------------------------------------------------------
//--------------ОПИСАНИЕ ВСТРАИВАЕМЫХ МЕТОДОВ ШАБЛОНА DERIVED HELPER--------------------
/// Получение имени контракта
template <typename Base, typename Helper>
Q_INLINE_TEMPLATE const char *DerivedHelper<Base, Helper>::netcomName() const {
  return Helper::netcomName();
}
//--------------------------------------------------------------------------------------
/// Получение экземпляра класса QMetaObject
template <typename Base, typename Helper>
Q_INLINE_TEMPLATE const QMetaObject *DerivedHelper<Base, Helper>::metaObject() const {
  return Helper::metaObject();
}
//--------------------------------------------------------------------------------------

} // namespace helpers
} // namespace netcom

