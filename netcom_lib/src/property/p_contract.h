/*! ***************************************************************************
* Проект: src.pro
*
* @file property/p_contract.h
*
* @brief: Заголовочный файл описания свойств для типов являющихся контрактом
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#pragma once

#include "p_base.h"

namespace netcom {
namespace properties {

/// Вспомогательный макрос тела шаблона свойства
#define INNER_PROPERTY_TEMPLATE_TYPE(T) \
  Property< \
    T, typename qtp::QEnableIf< \
      TypeManager<T>::is_contract \
    >::Type \
  >

template <typename T>
class INNER_PROPERTY_TEMPLATE_TYPE(T) : public BaseProperty<T> {
  Q_DISABLE_COPY(Property)
public:
  using TM              = typename BaseProperty<T>::TM;
  using BindingFunction = typename TM::BindingFunction;
  using PointerType     = typename TM::PointerType;
  using ValueType       = typename TM::ValueType;

  Q_DECL_CONSTEXPR explicit Property(const BindingFunction &function);
  virtual ~Property() Q_DECL_EQ_DEFAULT;

  /* ---реализация интерфейса AbstractProperty--- */
  Q_DECL_CONSTEXPR virtual int metaType() const Q_DECL_OVERRIDE;
  virtual QByteArray metaName() const Q_DECL_OVERRIDE;

  Q_DECL_CONSTEXPR virtual AbstractProperty *child(int index = 0) const Q_DECL_OVERRIDE;

  virtual bool clone(const Contract *source, Contract *dest) Q_DECL_OVERRIDE;
  virtual void clear(Contract *instance) Q_DECL_OVERRIDE;

  virtual QVariant toVariant(const Contract *instance, bool *ok = Q_NULLPTR) const Q_DECL_OVERRIDE;
  virtual bool fromVariant(Contract *instance, const QVariant &v, bool ptr_owner) Q_DECL_OVERRIDE;

  virtual void toStream(QDataStream &stream) const Q_DECL_OVERRIDE;
  virtual void fromStream(QDataStream &stream) Q_DECL_OVERRIDE;

private:
  using base = BaseProperty<T>;
};

//---------------------ОПИСАНИЕ ВСТРАИВАЕМЫХ МЕТОДОВ ШАБЛОНА----------------------------
template <typename T>
Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE INNER_PROPERTY_TEMPLATE_TYPE(T)::Property(
  const typename BaseProperty<T>::TM::BindingFunction &function) :
  BaseProperty<T>(function) { }
//--------------------------------------------------------------------------------------
/// Возврат мета типа свойства, привязанного к QMetaType::Type
template <typename T>
Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE int INNER_PROPERTY_TEMPLATE_TYPE(T)::metaType() const {
  return qMetaTypeId<Contract*>();
}
//--------------------------------------------------------------------------------------
/// Возврат имени мета типов свойств (в случае контейнеров, таких типов может быть несколько)
template <typename T>
Q_OUTOFLINE_TEMPLATE QByteArray INNER_PROPERTY_TEMPLATE_TYPE(T)::metaName() const {
  return ValueType().netcomName();
}
//--------------------------------------------------------------------------------------
/// Возврат внутреннего свойства (для контрактов неприменимо!)
template <typename T>
Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE AbstractProperty *INNER_PROPERTY_TEMPLATE_TYPE(T)::child(int) const {
  return Q_NULLPTR;
}
//--------------------------------------------------------------------------------------
/// Клонирование значения свойства из другого свойства
template <typename T>
Q_OUTOFLINE_TEMPLATE bool INNER_PROPERTY_TEMPLATE_TYPE(T)::clone(const Contract *source, Contract *dest) {
  const auto p_source = static_cast<PointerType>(base::getter(const_cast<Contract*>(source)));
  auto p_dest = base::getter(dest);
  if (p_source && p_dest) {
    return netcom::contractClone(p_source, p_dest);
  }
  return false;
}
//--------------------------------------------------------------------------------------
/// Выполняет сброс значения по умолчанию
template <typename T>
Q_OUTOFLINE_TEMPLATE void INNER_PROPERTY_TEMPLATE_TYPE(T)::clear(Contract *instance) {
  if (auto p = base::getter(instance)) {
    (reinterpret_cast<Contract*>(p))->netcomClear();
  }
}
//--------------------------------------------------------------------------------------
/// Преобразование внутреннего значения свойства в QVariant
template <typename T>
Q_OUTOFLINE_TEMPLATE QVariant INNER_PROPERTY_TEMPLATE_TYPE(T)::toVariant(
  const Contract *instance, bool *ok) const {
  auto p = base::getter(const_cast<Contract*>(instance));
  if (p) {
    utility::setBoolPtrValue(ok, true);
    return qVariantFromValue(reinterpret_cast<Contract*>(p));
  }
  utility::setBoolPtrValue(ok, false);
  return QVariant();
}
//--------------------------------------------------------------------------------------
/// Изменение внутреннего значения свойства из QVariant'a
template <typename T>
Q_INLINE_TEMPLATE bool INNER_PROPERTY_TEMPLATE_TYPE(T)::fromVariant(
  Contract *instance, const QVariant &v, bool ptr_owner) {
  if (auto p_source = dynamic_cast<const PointerType>(qvariant_cast<Contract*>(v))) {
    if (ptr_owner && TM::is_pointer) {
      base::setter(instance, p_source);
      return true;
    }
    const bool result = netcom::contractClone(p_source, base::getter(instance));
    if (ptr_owner) {
      delete p_source;
    }
    return result;
  }
  return false;
}
//--------------------------------------------------------------------------------------
/// Преобразование внутреннего значения в QDataStream
template <typename T>
Q_INLINE_TEMPLATE void INNER_PROPERTY_TEMPLATE_TYPE(T)::toStream(QDataStream &) const {
}
//--------------------------------------------------------------------------------------
/// Изменение внутреннего значения из QDataStream
template <typename T>
Q_INLINE_TEMPLATE void INNER_PROPERTY_TEMPLATE_TYPE(T)::fromStream(QDataStream &) {
}
//--------------------------------------------------------------------------------------

#undef INNER_PROPERTY_TEMPLATE_TYPE

} // namespace properties
} // namespace netcom
