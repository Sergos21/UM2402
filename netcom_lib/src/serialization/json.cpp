/*! ***************************************************************************
* Проект: src.pro
*
* @file serialization/json.cpp
*
* @brief: Файл исходных данных класса сериализатора для JSON-формата
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#include "json.h"

#include <QMetaProperty>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonParseError>

#include <QDateTime>
#include <QPointF>
#include <QSizeF>
#include <QRectF>

#include <qmath.h>

#include "src/factory.h"
#include "src/property/abstract_property.h"
//--------------------------------------------------------------------------------------
using namespace netcom;
using namespace netcom::properties;
//--------------------------------------------------------------------------------------
/// Внутренний класс для сериализации/десериализации данных в формате JSON
class JsonPrivate {
public:
  Q_DECL_CONSTEXPR static const char *IDENTIFIER        { "$contract" };
  Q_DECL_CONSTEXPR static const double FLOAT_PRECISION  { 6. };

  Q_DECL_CONSTEXPR static const char *DATE_FORMAT       { "dd.MM.yyyy" };
  Q_DECL_CONSTEXPR static const char *TIME_FORMAT       { "hh:mm:ss" };
  Q_DECL_CONSTEXPR static const char *DATETIME_FORMAT   { "dd.MM.yyyy hh:mm:ss" };

  Q_DECL_CONSTEXPR static const char *QPOINT_X_NAME     { "x" };
  Q_DECL_CONSTEXPR static const char *QPOINT_Y_NAME     { "y" };
  Q_DECL_CONSTEXPR static const char *QSIZE_WIDTH_NAME  { "width" };
  Q_DECL_CONSTEXPR static const char *QSIZE_HEIGHT_NAME { "height" };
  Q_DECL_CONSTEXPR static const char *QRECT_X_NAME      = QPOINT_X_NAME;
  Q_DECL_CONSTEXPR static const char *QRECT_Y_NAME      = QPOINT_Y_NAME;
  Q_DECL_CONSTEXPR static const char *QRECT_WIDTH_NAME  = QSIZE_WIDTH_NAME;
  Q_DECL_CONSTEXPR static const char *QRECT_HEIGHT_NAME = QSIZE_HEIGHT_NAME;

  static void writeContract(const Contract *contract, QJsonObject *json_object);
  static void readContract(Contract *contract, const QJsonObject *json_object);

  static QJsonValue variantTypeToJsonValue(const Contract *contract, const AbstractProperty *property,
                                           const QVariant &value = QVariant());
  static QJsonValue enumTypeToJsonValue(const Contract *contract, const AbstractProperty *property,
                                        const QVariant &value = QVariant());
  static QJsonValue contractTypeToJsonValue(const Contract *contract, const AbstractProperty *property,
                                            const QVariant &value = QVariant());
  static QJsonValue containerTypeToJsonValue(const Contract *contract, const AbstractProperty *property,
                                             const QVariant &value = QVariant());

  static QVariant variantTypeFromJsonValue(const QJsonValue &json_value, Contract *contract,
                                           AbstractProperty *property);
  static QVariant enumTypeFromJsonValue(const QJsonValue &json_value, Contract *contract,
                                        AbstractProperty *property);
  static QVariant contractTypeFromJsonValue(const QJsonValue &json_value, Contract *contract,
                                            AbstractProperty *property);
  static QVariant containerTypeFromJsonValue(const QJsonValue &json_value, Contract *contract,
                                             AbstractProperty *property);

  static double getFloatPrecision(double value, double precision);
  static bool jsonValueIsSimple(const QJsonValue &value);
};
//--------------------------------------------------------------------------------------
/// <summary> Сериализовать контракт в строку формата JSON </summary>
/// <param name="contract"> Указатель на сериализуемый контракт </param>
/// <param name="ok"> Результат преобразования (true/false) </param>
/// <returns> QByteArray </returns>
QByteArray netcom::serialization::ToJSON(const Contract *contract, bool *ok) {
  QByteArray data;
  if (contract) {
    QJsonDocument document;
    QJsonObject body { { JsonPrivate::IDENTIFIER, contract->netcomName() } };
    JsonPrivate::writeContract(contract, &body);
    document.setObject(body);
    data = document.toJson();
    utility::setBoolPtrValue(ok, true);
  }
  else {
    utility::setBoolPtrValue(ok, false);
  }
  return data;
}
//--------------------------------------------------------------------------------------
/// <summary> Десериализовать контракт из строки формата JSON </summary>
/// <param name="data"> Массив данных для десериализации </param>
/// <param name="ok"> Результат преобразования (true/false) </param>
/// <returns>
/// Созданный экземпляр Contract*, который должен быть удален пользователем!
///  </returns>
Contract *netcom::serialization::FromJSON(const QByteArray &data, bool *ok) {
  utility::setBoolPtrValue(ok, false);
  Contract *contract { Q_NULLPTR };
  if (!data.isEmpty()) {
    QJsonParseError json_error;
    auto object = QJsonDocument::fromJson(data, &json_error).object();
    if (json_error.error == QJsonParseError::NoError) {
      const QString contract_name = object.value(JsonPrivate::IDENTIFIER).toString();
      contract = Factory::createContractInstance(contract_name);
      if (contract) {
        JsonPrivate::readContract(contract, &object);
        utility::setBoolPtrValue(ok, true);
      }
    }
  }
  return contract;
}
//--------------------------------------------------------------------------------------
/// <summary> Попытка десериализовать данные в уже созданный контракт из строки формата JSON </summary>
/// <param name="contract"> Указатель на контракт подлежащий десериализации </param>
/// <param name="data"> Массив данных для десериализации </param>
/// <returns> Результат выполнения (true/fasle) </returns>
bool netcom::serialization::FromJSON(Contract *contract, const QByteArray &data) {
  if (!contract || data.isEmpty()) {
    return false;
  }

  contract->netcomClear();
  QJsonParseError json_error;
  auto object = QJsonDocument::fromJson(data, &json_error).object();
  if (json_error.error == QJsonParseError::NoError) {
    const QString contract_name = object.value(JsonPrivate::IDENTIFIER).toString();
    if (contract->netcomName() == contract_name) {
      JsonPrivate::readContract(contract, &object);
      return true;
    }
  }
  return false;
}
//--------------------------------------------------------------------------------------
/// <summary> Запись контракта в объект QJsonObject </summary>
/// <param name="contract"> Указатель на контракт подлежащий сериализации </param>
/// <param name="json_object"> Объект записи </param>
/// <returns></returns>
void JsonPrivate::writeContract(const Contract *contract, QJsonObject *json_object) {
  const QMetaObject * const mo  = contract->metaObject();
  const int contract_offset     = mo->superClass() ? mo->superClass()->propertyOffset() : mo->propertyOffset();
  for (int n = contract_offset; n < mo->propertyCount(); ++n) {
    const auto qt_property      = mo->property(n);
    const auto netcom_property  = qvariant_cast<AbstractProperty*>(qt_property.readOnGadget(contract));
    Q_ASSERT(netcom_property);

    if (netcom_property->isVariantCast()) {
      if (netcom_property->isEnum()) {
        json_object->insert(qt_property.name(), enumTypeToJsonValue(contract, netcom_property));
      }
      else {
        json_object->insert(qt_property.name(), variantTypeToJsonValue(contract, netcom_property));
      }
    }
    else if (netcom_property->isContract()) {
      json_object->insert(qt_property.name(), contractTypeToJsonValue(contract, netcom_property));
    }
    else if (netcom_property->isSequentialContainer()) {
      json_object->insert(qt_property.name(), containerTypeToJsonValue(contract, netcom_property));
    }
  }
}
//--------------------------------------------------------------------------------------
/// <summary> Чтение из объекта QJsonObject в контракт </summary>
/// <param name="contract"> Указатель на контракт подлежащий десериализации </param>
/// <param name="json_object"> Объект чтения </param>
/// <returns></returns>
void JsonPrivate::readContract(Contract *contract, const QJsonObject *json_object) {
  const QMetaObject * const mo  = contract->metaObject();
  const int contract_offset     = mo->superClass() ? mo->superClass()->propertyOffset() : mo->propertyOffset();
  for (int n = contract_offset; n < mo->propertyCount(); ++n) {
    const auto qt_property      = mo->property(n);
    if (!json_object->contains(qt_property.name()))
      continue;
    const auto netcom_property  = qvariant_cast<AbstractProperty*>(qt_property.readOnGadget(contract));
    const auto json_value       = json_object->value(qt_property.name());

    if (netcom_property->isVariantCast()) {
      if (netcom_property->isEnum()) {
        const QVariant value = enumTypeFromJsonValue(json_value, contract, netcom_property);
        netcom_property->fromVariant(contract, value, false);
      }
      else {
        const QVariant value = variantTypeFromJsonValue(json_value, contract, netcom_property);
        netcom_property->fromVariant(contract, value, false);
      }
    }
    else if (netcom_property->isContract() && json_value.isObject()) {
      const QVariant contract_variant = contractTypeFromJsonValue(json_value, contract, netcom_property);
      netcom_property->fromVariant(contract, contract_variant, false);
      // удаляем экземпляр созданного контракта
      delete qvariant_cast<Contract*>(contract_variant);
    }
    else if ((netcom_property->isSequentialContainer() ||
              netcom_property->isAssociativeContainer()) &&
             json_value.isArray()) {
      const QVariant container = containerTypeFromJsonValue(json_value, contract, netcom_property);
      // все элементы контейнера с указателем внутри QVariant передают управление непосредственно свойству контракта
      netcom_property->fromVariant(contract, container, true);
    }
  }
}
//--------------------------------------------------------------------------------------
/// <summary> Преобразование значения из QVariant в QJsonValue </summary>
/// <param name="contract"> Указатель на контракт подлежащий сериализации </param>
/// <param name="property"> Указатель на обрабатываемое свойство </param>
/// <param name="value"> Значение подлежащее сериализации </param>
/// <returns></returns>
QJsonValue JsonPrivate::variantTypeToJsonValue(const Contract *contract,
                                               const AbstractProperty *property,
                                               const QVariant &value) {
  QJsonValue result;
  if (property->isValid(contract)) {
    const QVariant &actual_value = value.isValid() ? value : property->toVariant(contract);
    switch (property->metaType()) {
    case QMetaType::QPoint:
    case QMetaType::QPointF:
      result = QJsonObject {
        { QPOINT_X_NAME, QJsonValue::fromVariant(actual_value.toPointF().x()) },
        { QPOINT_Y_NAME, QJsonValue::fromVariant(actual_value.toPointF().y()) }
      };
      break;
    case QMetaType::QSize:
    case QMetaType::QSizeF:
      result = QJsonObject {
        { QSIZE_WIDTH_NAME, QJsonValue::fromVariant(actual_value.toSizeF().width()) },
        { QSIZE_HEIGHT_NAME, QJsonValue::fromVariant(actual_value.toSizeF().height()) }
      };
      break;
    case QMetaType::QRect:
    case QMetaType::QRectF:
      result = QJsonObject {
        { QRECT_X_NAME, QJsonValue::fromVariant(actual_value.toRectF().x()) },
        { QRECT_Y_NAME, QJsonValue::fromVariant(actual_value.toRectF().y()) },
        { QRECT_WIDTH_NAME, QJsonValue::fromVariant(actual_value.toRectF().width()) },
        { QRECT_HEIGHT_NAME, QJsonValue::fromVariant(actual_value.toRectF().height()) }
      };
      break;
    case QMetaType::Float:
    case QMetaType::Double:
      result = getFloatPrecision(actual_value.toDouble(), FLOAT_PRECISION);
      break;
    case QMetaType::QDate:
      result = actual_value.toDate().toString(DATE_FORMAT);
      break;
    case QMetaType::QTime:
      result = actual_value.toTime().toString(TIME_FORMAT);
      break;
    case QMetaType::QDateTime:
      result = actual_value.toDateTime().toString(DATETIME_FORMAT);
      break;
    default:
      result = QJsonValue::fromVariant(actual_value);
    }
  }
  return result;
}
//--------------------------------------------------------------------------------------
/// <summary> Преобразование значения из перечисления в QJsonValue </summary>
/// <param name="contract"> Указатель на контракт подлежащий сериализации </param>
/// <param name="property"> Указатель на обрабатываемое свойство </param>
/// <param name="value"> Значение подлежащее сериализации </param>
/// <returns></returns>
QJsonValue JsonPrivate::enumTypeToJsonValue(const Contract *contract,
                                            const AbstractProperty *property,
                                            const QVariant &value) {
  QJsonValue result;
  if (property->isValid(contract)) {
    const QVariant &actual_value  = value.isValid() ? value : property->toVariant(contract);
    if (actual_value.canConvert<qint32>()) {
      const QMetaObject * const mo  = contract->metaObject();
      const QByteArray enum_name    = property->metaName();
      const QMetaEnum meta_enum     = mo->enumerator(mo->indexOfEnumerator(enum_name));
      if (meta_enum.isValid()) {
        result = QLatin1String(meta_enum.valueToKey(actual_value.toInt()));
      }
    }
  }
  return result;
}
//--------------------------------------------------------------------------------------
/// <summary> Преобразование контракта из QVariant в QJsonValue </summary>
/// <param name="contract"> Указатель на контракт подлежащий сериализации </param>
/// <param name="property"> Указатель на обрабатываемое свойство </param>
/// <param name="value"> Значение подлежащее сериализации </param>
/// <returns></returns>
QJsonValue JsonPrivate::contractTypeToJsonValue(const Contract *contract,
                                                const AbstractProperty *property,
                                                const QVariant &value) {
  if (!property->isValid(contract)) {
    return QJsonValue();
  }

  QJsonObject result { { JsonPrivate::IDENTIFIER, property->metaName().data() } };
  const QVariant &actual_value = value.isValid() ? value : property->toVariant(contract);
  if (const auto nested_contract = qvariant_cast<Contract*>(actual_value)) {
    writeContract(nested_contract, &result);
  }
  return result;
}
//--------------------------------------------------------------------------------------
/// <summary> Преобразование контейнера из QVariant в QJsonValue </summary>
/// <param name="contract"> Указатель на контракт подлежащий сериализации </param>
/// <param name="property"> Указатель на обрабатываемое свойство </param>
/// <param name="value"> Значение подлежащее сериализации </param>
/// <returns></returns>
QJsonValue JsonPrivate::containerTypeToJsonValue(const Contract *contract,
                                                 const AbstractProperty *property,
                                                 const QVariant &value) {
  QJsonArray result;
  if (property->isValid(contract)) {
    const AbstractProperty *inner_property = property->child();
    Q_ASSERT(inner_property);
    const QVariant actual_value = value.isValid() ? value : property->toVariant(contract);
    const int type = actual_value.userType();
    if (type == QVariant::List || type == QVariant::Map) {
      if (inner_property->isVariantCast()) {
        if (inner_property->isEnum()) {
          Q_FOREACH (const QVariant &element, actual_value.toList())
            result.append(enumTypeToJsonValue(contract, property, element));
        }
        else {
          Q_FOREACH (const QVariant &element, actual_value.toList())
            result.append(variantTypeToJsonValue(contract, property, element));
        }
      }
      else if (inner_property->isContract()) {
        Q_FOREACH (const QVariant &element, actual_value.toList())
          result.append(contractTypeToJsonValue(contract, property, element));
      }
      else if (inner_property->isSequentialContainer()) {
        Q_FOREACH (const QVariant &element, actual_value.toList())
          result.append(containerTypeToJsonValue(contract, property, element));
      }
    }
  }
  return result;
}
//--------------------------------------------------------------------------------------
/// <summary> Преобразование значения из QJsonValue в QVariant </summary>
/// <param name="json_value"> Значение подлежащее десериализации </param>
/// <param name="contract"> Указатель на контракт подлежащий десериализации </param>
/// <param name="property"> Указатель на обрабатываемое свойство </param>
/// <returns></returns>
QVariant JsonPrivate::variantTypeFromJsonValue(const QJsonValue &json_value,
                                               Contract *contract,
                                               AbstractProperty *property) {
  QVariant result;
  if (property->isValid(contract)) {
    switch (property->metaType()) {
    case QMetaType::QPoint:
    case QMetaType::QPointF:
      if (json_value.isObject()) {
        const QJsonObject json_object = json_value.toObject();
        result = QPointF { json_object.value(QPOINT_X_NAME).toDouble(),
                           json_object.value(QPOINT_Y_NAME).toDouble() };
      }
      break;
    case QMetaType::QSize:
    case QMetaType::QSizeF:
      if (json_value.isObject()) {
        const QJsonObject json_object = json_value.toObject();
        result = QSizeF { json_object.value(QSIZE_WIDTH_NAME).toDouble(),
                          json_object.value(QSIZE_HEIGHT_NAME).toDouble() };
      }
      break;
    case QMetaType::QRect:
    case QMetaType::QRectF:
      if (json_value.isObject()) {
        const QJsonObject json_object = json_value.toObject();
        result = QRectF { json_object.value(QRECT_X_NAME).toDouble(),
                          json_object.value(QRECT_Y_NAME).toDouble(),
                          json_object.value(QRECT_WIDTH_NAME).toDouble(),
                          json_object.value(QRECT_HEIGHT_NAME).toDouble() };
      }
      break;
    case QMetaType::QDate:
      if (json_value.isString()) {
        result = QDate::fromString(json_value.toString(), DATE_FORMAT);
      }
      break;
    case QMetaType::QTime:
      if (json_value.isString()) {
        result = QTime::fromString(json_value.toString(), TIME_FORMAT);
      }
      break;
    case QMetaType::QDateTime:
      if (json_value.isString()) {
        result = QDateTime::fromString(json_value.toString(), DATETIME_FORMAT);
      }
      break;
    default:
      if (jsonValueIsSimple(json_value)) {
        result = json_value.toVariant();
      }
    }
  }
  return result;
}
//--------------------------------------------------------------------------------------
/// <summary> Преобразование перечисления из QJsonValue в QVariant </summary>
/// <param name="json_value"> Значение подлежащее десериализации </param>
/// <param name="contract"> Указатель на перечисление подлежащее десериализации </param>
/// <param name="property"> Указатель на обрабатываемое свойство </param>
/// <returns></returns>
QVariant JsonPrivate::enumTypeFromJsonValue(const QJsonValue &json_value,
                                            Contract *contract,
                                            AbstractProperty *property) {
  QVariant result;
  if (property->isValid(contract) && jsonValueIsSimple(json_value)) {
    const QMetaObject * const mo  = contract->metaObject();
    const QByteArray enum_name    = property->metaName();
    const QMetaEnum meta_enum     = mo->enumerator(mo->indexOfEnumerator(enum_name.data()));
    if (meta_enum.isValid()) {
      const int enum_value = meta_enum.keysToValue(json_value.toString().toLatin1().data());
      if (enum_value > -1) {
        result = enum_value;
      }
    }
  }
  return result;
}
//--------------------------------------------------------------------------------------
/// <summary> Преобразование контракта из QJsonValue в QVariant </summary>
/// <param name="json_value"> Значение подлежащее десериализации </param>
/// <param name="contract"> Указатель на контракт подлежащий десериализации </param>
/// <param name="property"> Указатель на обрабатываемое свойство </param>
/// <returns></returns>
QVariant JsonPrivate::contractTypeFromJsonValue(const QJsonValue &json_value,
                                                Contract *contract,
                                                AbstractProperty *property) {
  if (property->isValid(contract)) {
    const QJsonObject json_object     = json_value.toObject();
    const QString new_contract_name   = json_object.value(JsonPrivate::IDENTIFIER).toString();
    if (property->metaName() == new_contract_name) {
      // указатель передается в привязанный объект для данного свойства!
      Contract *new_contract = Factory::createContractInstance(new_contract_name);
      if (new_contract) {
        readContract(new_contract, &json_object);
        return qVariantFromValue(new_contract);
      }
    }
  }
  return QVariant();
}
//--------------------------------------------------------------------------------------
/// <summary> Преобразование контейнера из QJsonValue в QVariant </summary>
/// <param name="json_value"> Значение подлежащее десериализации </param>
/// <param name="contract"> Указатель на контракт подлежащий десериализации </param>
/// <param name="property"> Указатель на обрабатываемое свойство </param>
/// <returns></returns>
QVariant JsonPrivate::containerTypeFromJsonValue(const QJsonValue &json_value,
                                                 Contract *contract,
                                                 AbstractProperty *property) {
  QVariantList variant_container;
  if (property->isValid(contract)) {
    const AbstractProperty *inner_property = property->child();
    Q_ASSERT(inner_property);
    const QJsonArray json_array = json_value.toArray();
    Q_FOREACH (const QJsonValue &element, json_array) {
      if (jsonValueIsSimple(element)) {
        if (inner_property->isVariantCast()) {
          variant_container << variantTypeFromJsonValue(element, contract, property);
        }
        else if (inner_property->isEnum()) {
          variant_container << enumTypeFromJsonValue(element, contract, property);
        }
      }
      else if (element.isObject() && inner_property->isContract()) {
        variant_container << contractTypeFromJsonValue(element, contract, property);
      }
      else if (element.isArray() && inner_property->isSequentialContainer()) {
        variant_container << containerTypeFromJsonValue(element, contract, property);
      }
    }
  }
  return variant_container;
}
//--------------------------------------------------------------------------------------
/// <summary> Преобразование числа с плавающей запятой с заданной точностью </summary>
/// <param name="value"> Значение для преобразования </param>
/// <param name="precision"> Точность преобразования </param>
/// <returns> double </returns>
double JsonPrivate::getFloatPrecision(double value, double precision) {
  return (qFloor((value * qPow(10., precision) + 0.5)) / qPow(10., precision));
}
//--------------------------------------------------------------------------------------
/// <summary> Проверка валидности значения пришедшего в QJsonValue </summary>
/// <param name="value"> Значение для проверки </param>
/// <returns> bool </returns>
bool JsonPrivate::jsonValueIsSimple(const QJsonValue &value) {
  return (value.isBool() || value.isDouble() || value.isString());
}
//--------------------------------------------------------------------------------------
