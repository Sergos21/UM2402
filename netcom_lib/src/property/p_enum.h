/*! ***************************************************************************
* Проект: src.pro
*
* @file property/p_enum.h
*
* @brief: Заголовочный файл описания свойств являющихся enum'ами
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
      TypeManager<T>::is_variant_cast && TypeManager<T>::is_enum \
    >::Type \
  >

template <typename T>
class INNER_PROPERTY_TEMPLATE_TYPE(T) : public BaseProperty<T> {
  Q_DISABLE_COPY(Property)
public:
  using TM              = typename BaseProperty<T>::TM;
  using BindingFunction = typename TM::BindingFunction;
  using ValueType       = typename TM::ValueType;
  using PointerType     = typename TM::PointerType;

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
  using base    = BaseProperty<T>;
  using enum_ut = typename std::underlying_type<T>::type;
};

//---------------------------ОПИСАНИЕ МЕТОДОВ ШАБЛОНА-----------------------------------
template <typename T>
Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE INNER_PROPERTY_TEMPLATE_TYPE(T)::Property(
  const typename BaseProperty<T>::TM::BindingFunction &function) :
  BaseProperty<T>(function) { }
//--------------------------------------------------------------------------------------
/// Возврат мета типа свойства, привязанного к QMetaType::Type
template <typename T>
Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE int INNER_PROPERTY_TEMPLATE_TYPE(T)::metaType() const {
  return qMetaTypeId<ValueType>();
}
//--------------------------------------------------------------------------------------
/// Возврат имени мета типа свойства
template <typename T>
Q_OUTOFLINE_TEMPLATE QByteArray INNER_PROPERTY_TEMPLATE_TYPE(T)::metaName() const {
  static const QByteArray name = QByteArray(QMetaType::typeName(metaType())).split(':').last();
  return name;
}
//--------------------------------------------------------------------------------------
/// Возврат внутреннего свойства (для типов преобразуемых через QVariant неприменимо!)
template <typename T>
Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE AbstractProperty *INNER_PROPERTY_TEMPLATE_TYPE(T)::child(int) const {
  return Q_NULLPTR;
}
//--------------------------------------------------------------------------------------
/// Клонирование значения свойства из другого свойства
template <typename T>
Q_OUTOFLINE_TEMPLATE bool INNER_PROPERTY_TEMPLATE_TYPE(T)::clone(const Contract *source, Contract *dest) {
  auto p_source = base::getter(const_cast<Contract*>(source));
  return base::setter(dest, *p_source);
}
//--------------------------------------------------------------------------------------
/// Выполняет сброс значения по умолчанию
template <typename T>
Q_OUTOFLINE_TEMPLATE void INNER_PROPERTY_TEMPLATE_TYPE(T)::clear(Contract *instance) {
  static ValueType default_value;
  base::setter(instance, default_value);
}
//--------------------------------------------------------------------------------------
/// Преобразование внутреннего значения свойства в QVariant
template <typename T>
Q_OUTOFLINE_TEMPLATE QVariant INNER_PROPERTY_TEMPLATE_TYPE(T)::toVariant(
  const Contract *instance, bool *ok) const {
  utility::setBoolPtrValue(ok, true);
  auto p = base::getter(const_cast<Contract*>(instance));
  return (p ? utility::enum_value(*p) : QVariant());
}
//--------------------------------------------------------------------------------------
/// Изменение внутреннего значения свойства из QVariant'a
template <typename T>
Q_OUTOFLINE_TEMPLATE bool INNER_PROPERTY_TEMPLATE_TYPE(T)::fromVariant(
  Contract *instance, const QVariant &v, bool) {
  if (v.isValid() && !v.isNull()) {
    ValueType value = utility::enum_value<T>(qvariant_cast<enum_ut>(v));
    return base::setter(instance, value);
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
