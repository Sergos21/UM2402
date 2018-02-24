/*! ***************************************************************************
* Проект: src.pro
*
* @file property/p_base.h
*
* @brief: Вспомогательный класс реализующий основу для свойства контракта
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#pragma once

#include "abstract_property.h"
#include "inner_value.h"
#include "src/type_manager.h"

namespace netcom {
namespace properties {

//--------------------------ПРОТОТИПЫ ШАБЛОННЫХ КЛАССОВ---------------------------------
/// прототип шаблона описывающего свойства в классе-контракте
template <typename T, typename Enabled = void> class Property;
//--------------------------------------------------------------------------------------

template <typename T>
class BaseProperty : public AbstractProperty {
  Q_DISABLE_COPY(BaseProperty)
public:
  using TM              = TypeManager<T>;
  using BindingFunction = typename TM::BindingFunction;
  using ReferenceType   = typename TM::ReferenceType;
  using PointerType     = typename TM::PointerType;

  Q_DECL_CONSTEXPR explicit BaseProperty(const BindingFunction &function);
  virtual ~BaseProperty() Q_DECL_EQ_DEFAULT;

  Q_DECL_CONSTEXPR virtual bool isPOD() const Q_DECL_OVERRIDE;
  Q_DECL_CONSTEXPR virtual bool isPointer() const Q_DECL_OVERRIDE;
  Q_DECL_CONSTEXPR virtual bool isConst() const Q_DECL_OVERRIDE;
  Q_DECL_CONSTEXPR virtual bool isVariantCast() const Q_DECL_OVERRIDE;
  Q_DECL_CONSTEXPR virtual bool isEnum() const Q_DECL_OVERRIDE;
  Q_DECL_CONSTEXPR virtual bool isContract() const Q_DECL_OVERRIDE;
  Q_DECL_CONSTEXPR virtual bool isSequentialContainer() const Q_DECL_OVERRIDE;
  Q_DECL_CONSTEXPR virtual bool isAssociativeContainer() const Q_DECL_OVERRIDE;

  virtual bool isValid(const Contract *instance) const Q_DECL_OVERRIDE;

protected:
  using value = inner::value<T>;

  PointerType getter(Contract *instance) const;
  bool setter(Contract *instance, ReferenceType v);
  bool setter(Contract *instance, PointerType v);

  const BindingFunction p_function;
};

//---------------------------ОПИСАНИЕ МЕТОДОВ ШАБЛОНА-----------------------------------
template <typename T>
Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE BaseProperty<T>::BaseProperty(
  const typename BaseProperty<T>::BindingFunction &function) :
  p_function(function) { }
//--------------------------------------------------------------------------------------
/// Проверка является ли данное свойство простым POD-типом (int, float, char etc.)
template <typename T>
Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE bool BaseProperty<T>::isPOD() const {
  return TM::is_pod;
}
//--------------------------------------------------------------------------------------
/// Проверка является ли данное свойство указателем
template <typename T>
Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE bool BaseProperty<T>::isPointer() const {
  return TM::is_pointer;
}
//--------------------------------------------------------------------------------------
/// Проверка является ли данное свойство константой
template <typename T>
Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE bool BaseProperty<T>::isConst() const {
  return TM::is_const;
}
//--------------------------------------------------------------------------------------
/// Проверка является ли данное свойство преобразуемым в QVariant
template <typename T>
Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE bool BaseProperty<T>::isVariantCast() const {
  return TM::is_variant_cast;
}
//--------------------------------------------------------------------------------------
/// Проверка является ли данное свойство перечислением
template <typename T>
Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE bool BaseProperty<T>::isEnum() const {
  return TM::is_enum;
}
//--------------------------------------------------------------------------------------
/// Проверка является ли данное свойство другим контрактом
template <typename T>
Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE bool BaseProperty<T>::isContract() const {
  return TM::is_contract;
}
//--------------------------------------------------------------------------------------
/// Проверка является ли данное свойство последовательным контейнером
template <typename T>
Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE bool BaseProperty<T>::isSequentialContainer() const {
  return TM::is_sequential_container;
}
//--------------------------------------------------------------------------------------
/// Проверка является ли данное свойство ассоциативным контейнером
template <typename T>
Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE bool BaseProperty<T>::isAssociativeContainer() const {
  return TM::is_associative_container;
}
//--------------------------------------------------------------------------------------
/// Проверка является ли свойство валидным
template <typename T>
Q_OUTOFLINE_TEMPLATE bool BaseProperty<T>::isValid(const Contract *instance) const {
  return getter(const_cast<Contract*>(instance));
}
//--------------------------------------------------------------------------------------
/// Получение указателя на связанные со свойством данные
template <typename T>
Q_OUTOFLINE_TEMPLATE typename BaseProperty<T>::PointerType BaseProperty<T>::getter(
  Contract *instance) const {
  return value::ref(p_function(instance));
}
//--------------------------------------------------------------------------------------
/// Запись значения переданного ссылкой в связанные со свойством данные
template <typename T>
Q_OUTOFLINE_TEMPLATE bool BaseProperty<T>::setter(
  Contract *instance, typename BaseProperty<T>::ReferenceType v) {
  if (!TM::is_const && isValid(instance)) {
    value::deref(p_function(instance)) = v;
    return true;
  }
  return false;
}
//--------------------------------------------------------------------------------------
/// Запись значения переданного указателем в связанные со свойством данные
template <typename T>
Q_OUTOFLINE_TEMPLATE bool BaseProperty<T>::setter(
  Contract *instance, typename BaseProperty<T>::PointerType v) {
  if (!TM::is_const) {
    bool is_valid = false;
    typename TM::PointerRefType p = value::base(is_valid, p_function(instance));
    if (is_valid) {
      p = v;
      return true;
    }
  }
  return false;
}
//--------------------------------------------------------------------------------------

} // namespace properties
} // namespace netcom



