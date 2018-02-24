#pragma once

#include "project_element.h"

namespace data {

NETCOM_CONTRACT(Project, Project)
{
  Q_GADGET
public:
  QString name;                     // имя проекта
  NETCOM_PROPERTY(name)

  QString description;              // описание проекта
  NETCOM_PROPERTY(description)

  QList<ProjectElement> elements;   // элементы проекта
};

} // namespace data
