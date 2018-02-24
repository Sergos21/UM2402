#pragma once

#include "project_element.h"

namespace data {
namespace map {

enum class MapGameMode {
  TurnBase,
  RealTime
};

enum class MapCellType {
  Tile,
  Hexagonal,
  Fractal
};

enum class MapLogicalLevel {
  Tactical,
  Strategic,
  Global,
  Custom
};

NETCOM_DERIVED_CONTRACT(Map, ProjectElement, GameMap)
{
  Q_GADGET
public:
  QString name;                   // имя карты
  NETCOM_PROPERTY(name)

  QString description;            // описание карты
  NETCOM_PROPERTY(description)

  MapGameMode mode;               // игровой режим карты
  //NETCOM_PROPERTY(mode)

  MapCellType cell_type;          // тип ячейки на карте
  //NETCOM_PROPERTY(cell_type)

  int x;                          // размер карты по оси Х в условных ячейках
  NETCOM_PROPERTY(x)

  int y;                          // размер карты по оси Y в условных ячейках
  NETCOM_PROPERTY(y)

  bool is_single_layered;         // является ли карта однослойной
  NETCOM_PROPERTY(is_single_layered)

  MapLogicalLevel logical_level;  // логический уровень карты
  //NETCOM_PROPERTY(logical_level)

};

} // namespace map
} // namespace data
