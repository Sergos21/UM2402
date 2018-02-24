/*! ***************************************************************************
* Проект: src.pro
*
* @file contract.cpp
*
* @brief: Файл исходных данных класса контракта
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#include "contract.h"
#include "property/abstract_property.h"

#include <QMetaProperty>
//--------------------------------------------------------------------------------------
using namespace netcom;
//--------------------------------------------------------------------------------------
const int Contract::netcom_variant_type { qRegisterMetaType<Contract*>() };
//--------------------------------------------------------------------------------------
bool netcom::contractClone(const Contract *source, Contract *dest) {
  if (!source || !dest) {
    return false;
  }

  const QMetaObject * const mo_source = source->metaObject();
  const QMetaObject * const mo_dest   = dest->metaObject();

  if (mo_source != mo_dest) {
    return false;
  }

  for (int n = mo_source->propertyOffset(); n < mo_source->propertyCount(); ++n) {
    auto property = qvariant_cast<properties::AbstractProperty*>(mo_source->property(n).readOnGadget(source));;
    Q_ASSERT(property);
    if (!property->clone(source, dest)) {
      return false;
    }
  }
  return true;
}
//--------------------------------------------------------------------------------------
Contract::Contract(const Contract &other) {
  *this = other;
}
//--------------------------------------------------------------------------------------
Contract &Contract::operator=(const Contract &other) {
  if (this != &other) {
    netcomClone(&other);
  }
  return *this;
}
//--------------------------------------------------------------------------------------
bool Contract::netcomClone(const Contract *source) {
  return contractClone(source, this);
}
//--------------------------------------------------------------------------------------
void Contract::netcomClear() {
  const QMetaObject * const mo = this->metaObject();
  for (int n = mo->propertyOffset(); n < mo->propertyCount(); ++n) {
    auto property = qvariant_cast<properties::AbstractProperty*>(mo->property(n).readOnGadget(this));
    Q_ASSERT(property);
    property->clear(this);
  }
}
//--------------------------------------------------------------------------------------
