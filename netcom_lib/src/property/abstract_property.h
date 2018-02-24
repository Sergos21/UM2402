/*! ***************************************************************************
* Проект: src.pro
*
* @file property/abstract_property.h
*
* @brief: Базовый интерфейс абстрактного свойства для контракта
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#pragma once

#include <QVariant>

#include "src/common.h"

namespace netcom {

class Contract;

namespace properties {

class NETCOM_SHARED_EXPORT AbstractProperty {
  Q_DISABLE_COPY(AbstractProperty)
public:
  Q_DECL_CONSTEXPR AbstractProperty() Q_DECL_EQ_DEFAULT;
  virtual ~AbstractProperty() Q_DECL_EQ_DEFAULT;

  /// Проверка является ли данное свойство простым POD-типом (int, float, char etc.)
  virtual bool isPOD() const = 0;
  /// Проверка является ли данное свойство указателем
  virtual bool isPointer() const = 0;
  /// Проверка является ли данное свойство константой
  virtual bool isConst() const = 0;
  /// Проверка является ли данное свойство преобразуемым в QVariant
  virtual bool isVariantCast() const = 0;
  /// Проверка является ли данное свойство перечислением
  virtual bool isEnum() const = 0;
  /// Проверка является ли данное свойство другим контрактом
  virtual bool isContract() const = 0;
  /// Проверка является ли данное свойство последовательным контейнером
  virtual bool isSequentialContainer() const = 0;
  /// Проверка является ли данное свойство ассоциативным контейнером
  virtual bool isAssociativeContainer() const = 0;
  /// Проверка является ли свойство валидным
  virtual bool isValid(const Contract *instance) const = 0;

  /// Возврат мета типа свойства, привязанного к QMetaType::Type
  virtual int metaType() const = 0;
  /// Возврат имени мета типа свойства
  virtual QByteArray metaName() const = 0;

  /// Возврат внутреннего свойства (например, для контейнеров)
  virtual AbstractProperty *child(int index = 0) const = 0;

  /// Клонирование значения свойства из другого свойства
  virtual bool clone(const Contract *source, Contract *dest) = 0;
  /// Выполняет сброс значения по умолчанию
  virtual void clear(Contract *instance) = 0;

  /// Преобразование внутреннего значения свойства в QVariant
  virtual QVariant toVariant(const Contract *instance, bool *ok = Q_NULLPTR) const = 0;
  /// Изменение внутреннего значения свойства из QVariant'a
  virtual bool fromVariant(Contract *instance, const QVariant &v, bool ptr_owner) = 0;

  /// Преобразование внутреннего значения в QDataStream
  virtual void toStream(QDataStream &stream) const = 0;
  /// Изменение внутреннего значения из QDataStream
  virtual void fromStream(QDataStream &stream) = 0;
};

} // namespace properties
} // namespace netcom

Q_DECLARE_METATYPE(netcom::properties::AbstractProperty*)
