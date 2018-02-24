/*! ***************************************************************************
* Проект: src.pro
*
* @file property/p_s_container.h
*
* @brief: Заголовочный файл описания свойств для типов являющихся
* последовательным контейнером (QVector, QList и т.д)
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#pragma once

#include "p_base.h"

#include <QDebug>

namespace netcom {
namespace properties {

/// Вспомогательный макрос тела шаблона свойства
#define INNER_PROPERTY_TEMPLATE_TYPE(T) \
  Property< \
    T, typename qtp::QEnableIf< \
      TypeManager<T>::is_sequential_container \
    >::Type \
  >

template <typename T>
class INNER_PROPERTY_TEMPLATE_TYPE(T) : public BaseProperty<T> {
  Q_DISABLE_COPY(Property)
public:
  using TM              = typename BaseProperty<T>::TM;
  using BindingFunction = typename TM::BindingFunction;
  using ValueType       = typename TM::ValueType;
  using PValueType      = typename std::decay<typename ValueType::value_type>::type;
  using PSizeType       = typename ValueType::size_type;

  /* реализация интерфейса AbstractProperty */
  Q_DECL_CONSTEXPR explicit Property(const BindingFunction &function);
  virtual ~Property() Q_DECL_EQ_DEFAULT;

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
  using base         = BaseProperty<T>;
  using Property_TM  = typename Property<PValueType>::TM;
  using PBindingType = typename Property_TM::BindingType;

  mutable PValueType m_current_value { };
  mutable Property<PValueType> m_value_property;
};
//---------------------------ОПИСАНИЕ МЕТОДОВ ШАБЛОНА-----------------------------------
template <typename T>
Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE INNER_PROPERTY_TEMPLATE_TYPE(T)::Property(
  const typename BaseProperty<T>::TM::BindingFunction &function) :
  BaseProperty<T>(function),
  m_value_property(
    [&](Contract *) -> PBindingType {
      return m_current_value;
    }
  ) { }
//--------------------------------------------------------------------------------------
/// Возврат мета типа свойства, привязанного к QMetaType::Type
template <typename T>
Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE int INNER_PROPERTY_TEMPLATE_TYPE(T)::metaType() const {
  return qMetaTypeId<QVariantList>();
}
//--------------------------------------------------------------------------------------
/// Возврат имени мета типа свойства
template <typename T>
Q_INLINE_TEMPLATE QByteArray INNER_PROPERTY_TEMPLATE_TYPE(T)::metaName() const {
  return QLatin1String("QVariantList").latin1();
}
//--------------------------------------------------------------------------------------
/// Возврат внутреннего свойства
template <typename T>
Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE AbstractProperty *INNER_PROPERTY_TEMPLATE_TYPE(T)::child(int index) const {
  return (0 == index ? &m_value_property : Q_NULLPTR);
}
//--------------------------------------------------------------------------------------
/// Клонирование значения свойства из другого свойства
template <typename T>
Q_OUTOFLINE_TEMPLATE bool INNER_PROPERTY_TEMPLATE_TYPE(T)::clone(const Contract *source, Contract *dest) {
  auto p_dest = base::getter(dest);
  if (!p_dest) {
    return false;
  }

  clear(dest);
  bool result = true;
  for (int n = 0; n < p_dest->size(); ++n) {
    if (!m_value_property.clone(source, dest)) {
      result = false;
      break;
    }
  }
  return result;
}
//--------------------------------------------------------------------------------------
/// Выполняет сброс значения по умолчанию на весь контейнер
template <typename T>
Q_OUTOFLINE_TEMPLATE void INNER_PROPERTY_TEMPLATE_TYPE(T)::clear(Contract *instance) {
  auto p = base::getter(instance);
  if (!TM::is_const && p) {
    std::for_each(p->begin(), p->end(), inner::value<PValueType>::destroy);
    p->clear();
  }
}
//--------------------------------------------------------------------------------------
/// Преобразование внутреннего контейнера свойства в QVariant
template <typename T>
Q_OUTOFLINE_TEMPLATE QVariant INNER_PROPERTY_TEMPLATE_TYPE(T)::toVariant(
  const Contract *instance, bool *ok) const {
  QVariantList result;
  if (auto p = base::getter(const_cast<Contract*>(instance))) {
    for (int n = 0; n < p->size(); ++n) {
      m_current_value = (*p)[n];
      result << m_value_property.toVariant(instance, ok);
      if (ok != Q_NULLPTR && *ok == false) {
        result.clear();
        break;
      }
    }
  }
  m_current_value = { };
  return result;
}
//--------------------------------------------------------------------------------------
/// Изменение внутреннего значения свойства из QVariant'a
template <typename T>
Q_OUTOFLINE_TEMPLATE bool INNER_PROPERTY_TEMPLATE_TYPE(T)::fromVariant(
  Contract *instance, const QVariant &v, bool ptr_owner) {
  auto p = base::getter(instance);
  if (!v.isValid() || !p) {
    return false;
  }

  const auto variant_list = qvariant_cast<QVariantList>(v);
  clear(instance);
  p->reserve(variant_list.size());

  for (int n = 0; n < variant_list.size(); ++n) {
    m_value_property.fromVariant(instance, variant_list[n], ptr_owner);
    p->push_back(m_current_value);
  }
  m_current_value = { };
  return true;
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

