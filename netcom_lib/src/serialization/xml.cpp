/*! ***************************************************************************
* Проект: src.pro
*
* @file serialization/xml.cpp
*
* @brief: Файл исходных данных класса сериализатора для XML-формата
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#include "xml.h"

#include <QMetaProperty>
#include <QXmlStreamWriter>

#include "src/contract.h"
#include "src/property/abstract_property.h"
#include "src/protocol/abstract_protocol.h"

using namespace netcom;
using namespace netcom::properties;
using namespace netcom::serialization::inner;

namespace xml_private {
  static void writeContract(const QString &name, const Contract *contract, QXmlStreamWriter *xml);
} // xml_private

//--------------------------------------------------------------------------------------
QByteArray XML::serialize(const Contract *contract, bool *ok) {
  QByteArray data;
  if (contract) {
    QXmlStreamWriter xml(&data);
    xml.setAutoFormatting(true);
    xml.setCodec("UTF-8");
    xml.writeStartDocument("1.0");
    xml_private::writeContract(contract->netcomName(), contract, &xml);
    xml.writeEndDocument();
    utility::setBoolPtrValue(ok, true);
  }
  else {
    utility::setBoolPtrValue(ok, false);
  }
  return data;
}
//--------------------------------------------------------------------------------------
Contract *XML::deserialize(const QByteArray &data, bool *ok) {
  Q_UNUSED(data)
  Q_UNUSED(ok)
  return Q_NULLPTR;
}
//--------------------------------------------------------------------------------------
bool XML::tryDeserialize(Contract *contract, const QByteArray &data) {
  Q_UNUSED(contract)
  Q_UNUSED(data)
  return false;
}
//--------------------------------------------------------------------------------------
void xml_private::writeContract(const QString &name, const Contract *contract, QXmlStreamWriter *xml) {
  xml->writeStartElement(name);
  const QMetaObject * const mo  = contract->metaObject();
  const int contract_offset     = mo->superClass() ? mo->superClass()->propertyOffset() : mo->propertyOffset();
  for (int n = contract_offset; n < mo->propertyCount(); ++n) {
    const auto property         = mo->property(n);
    const auto netcom_property  = qvariant_cast<AbstractProperty*>(property.readOnGadget(contract));

    if (netcom_property->isPOD()) {
      xml->writeTextElement(property.name(), netcom_property->toVariant(contract).toString());
    }
  }
  xml->writeEndElement();
}
