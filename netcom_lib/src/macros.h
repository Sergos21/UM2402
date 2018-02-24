/*! ***************************************************************************
* Проект: src.pro
*
* @file macros.h
*
* @brief: Файл описания макросов netcom_lib
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#pragma once

#include <QtCore/qglobal.h>

/// Внутренний макрос генерирующий имя свойства
#define INNER_NETCOM_PROPERTY(NAME) _property_ ## NAME ## _

/// Внутренние макросы для создания последовательности символов из строки
/// ---------------------------------------------------------------------
#define INNER_GET_CHAR_1(str, i) \
  (sizeof(str) > (i) ? str[(i)] : 0)

#define INNER_GET_CHAR_4(str, i) \
  INNER_GET_CHAR_1(str, i + 0), \
  INNER_GET_CHAR_1(str, i + 1), \
  INNER_GET_CHAR_1(str, i + 2), \
  INNER_GET_CHAR_1(str, i + 3)

#define INNER_GET_CHAR_16(str, i) \
  INNER_GET_CHAR_4(str, i + 0), \
  INNER_GET_CHAR_4(str, i + 4), \
  INNER_GET_CHAR_4(str, i + 8), \
  INNER_GET_CHAR_4(str, i + 12)

#define INNER_GET_CHAR_64(str, i) \
  INNER_GET_CHAR_16(str, i + 0), \
  INNER_GET_CHAR_16(str, i + 16), \
  INNER_GET_CHAR_16(str, i + 32), \
  INNER_GET_CHAR_16(str, i + 48)

#define INNER_GET_SEQUENCE(str) INNER_GET_CHAR_64(str, 0), 0 // guard for longer strings
/// ---------------------------------------------------------------------

/// Основной макрос необходимый для создания и авторегистрации контракта
/// на базе макроса Q_GADGET
/// ПЕРЕДАЕТСЯ:
///   TYPE_NAME - имя класса в конструкции языка С++,
///   CONTRACT_NAME - уникальное имя контракта в строковом варианте
#define NETCOM_CONTRACT(TYPE_NAME, CONTRACT_NAME) \
  class TYPE_NAME : public netcom::helpers::BaseHelper<TYPE_NAME, INNER_GET_SEQUENCE(#CONTRACT_NAME)>

/// Основной макрос необходимый для создания и авторегистрации контракта,
/// который может наследовать от одного указанного пользователем класса либо
/// контракта (может использоваться макрос Q_GADGET или Q_OBJECT)
/// ПЕРЕДАЕТСЯ:
///   TYPE_NAME - имя класса в конструкции языка С++,
///   BASE_TYPE_NAME - имя класса, от которого необходимо отнаследоваться в
///     конструкции языка С++
///   CONTRACT_NAME - уникальное имя контракта в строковом варианте
#define NETCOM_DERIVED_CONTRACT(TYPE_NAME, BASE_TYPE_NAME, CONTRACT_NAME) \
  class TYPE_NAME : public netcom::helpers::DerivedHelper< \
    BASE_TYPE_NAME, netcom::helpers::BaseHelper<TYPE_NAME, INNER_GET_SEQUENCE(#CONTRACT_NAME)> >

/// Макрос создания свойства контракта
/// ПЕРЕДАЕТСЯ:
///   NAME - имя реального члена класса
#define NETCOM_PROPERTY(NAME) \
private: \
  Q_PROPERTY(netcom::properties::AbstractProperty* NAME READ INNER_NETCOM_PROPERTY(NAME)) \
public : \
  auto INNER_NETCOM_PROPERTY(NAME)() -> netcom::properties::Property<decltype(NAME)>* { \
    using Type = decltype(NAME); \
    using TM = netcom::TypeManager<Type>; \
    using ContractType = std::remove_reference<decltype(*this)>::type; \
    static netcom::properties::Property<Type> property { \
      [&](Contract *instance) -> TM::BindingType { \
        Q_ASSERT(instance); \
        return (reinterpret_cast<ContractType*>(instance))->NAME; } \
    }; \
    return &property; \
  }
