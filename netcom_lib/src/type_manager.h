/*! ***************************************************************************
* Проект: src.pro
*
* @file type_manager.h
*
* @brief: Заголовочный файл вспомогательного класса для определения сигнатуры
* свойства
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#pragma once

#include <QMetaType>
#include <QVariant>

#include <functional>

#include "contract.h"

namespace netcom {

template <typename T>
struct TypeManager {
  /// Проверка на ссылочность типа
  Q_STATIC_ASSERT_X(!std::is_reference<T>::value,
                    "Reference types is not supported for netcom property");

  /// Проверка на volatile типа
  Q_STATIC_ASSERT_X(!std::is_volatile<T>::value,
                    "Volatile types is not supported for netcom property");

  /// Определяет базовый тип, убирая константность
  using BaseType        = typename std::decay<T>::type;
  /// Определяет тип значения
  using ValueType       = typename std::remove_pointer<BaseType>::type;
  /// Определяет ссылочный тип для значения
  using ReferenceType   = typename std::add_lvalue_reference<ValueType>::type;
  /// Определяет указатель на тип для значения
  using PointerType     = typename std::add_pointer<ValueType>::type;
  /// Определяет указатель на ссылочный тип для значения
  using PointerRefType  = typename std::add_lvalue_reference<PointerType>::type;

  /// Определяет является ли данный тип простым POD-типом (int, float, char etc.)
  static const bool is_pod = std::is_pod<T>::value;

  /// Определяет является ли данный тип указателем
  static const bool is_pointer = std::is_pointer<T>::value;

  /// Определяет является ли данный тип константой
  static const bool is_const = std::is_const<T>::value;

  /// Определяет является ли данный тип преобразуемым в QVariant
  static const bool is_variant_cast = std::is_constructible<QVariant, ValueType>::value &&
    std::is_convertible<ValueType, QVariant>::value;

  /// Определяет является ли данный тип мета-перечислением
  static const bool is_enum = std::is_enum<T>::value && qtp::IsQEnumHelper<T>::Value;

  /// Определяет является ли данный тип контрактом
  static const bool is_contract = std::is_base_of<Contract, ValueType>::value;

  /// Определяет является ли данный тип последовательным контейнером
  static const bool is_sequential_container = qtp::IsSequentialContainer<ValueType>::Value;

  /// Определяет является ли данный тип ассоциативным контейнером
  static const bool is_associative_container = qtp::IsAssociativeContainer<ValueType>::Value;

  /// Определяет тип возвращаемый из функции привязки к значению свойства
  using BindingType     = typename std::conditional<
    is_pointer, PointerRefType, ReferenceType>::type;

  /// Определяет вид функции привязки к значению свойства
  using BindingFunction = std::function<BindingType(Contract*)>;
};

} // namespace netcom


