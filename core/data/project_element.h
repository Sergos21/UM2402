#pragma once

#include "netcom_c11/netcom_core.h"

namespace data {

enum class ProjectElementType {
  Map
};

NETCOM_CONTRACT(ProjectElement, ProjectElement)
{
  Q_GADGET
public:
  ProjectElementType type;
  //NETCOM_PROPERTY(type)
};

} // namespace data
