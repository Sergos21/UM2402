/*! ***************************************************************************
* Проект: src.pro
*
* @file property/getter_setter.h
*
* @brief: Заголовочный файл вспомогательного элементов, которые могут принимать
* две формы - ссылку на объект или указатель на объект
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#pragma once

#include "src/type_manager.h"

namespace netcom {
namespace properties {
namespace inner {

/// Вспомогательный шаблон описывающий правила работы со свойством являющимся указателем
template <typename T, bool = TypeManager<T>::is_pointer>
struct value : std::unary_function<T, void> {
  using TM              = TypeManager<T>;
  using BindingType     = typename TM::BindingType;
  using PointerType     = typename TM::PointerType;
  using ReferenceType   = typename TM::ReferenceType;

  /// Правила создания экземпляра для данного типа
  Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE static PointerType create() {
    return new typename TM::ValueType { };
  }

  /// Правила разыменования переданного экземпляра для данного типа
  Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE static ReferenceType deref(BindingType t) {
    return *t;
  }

  /// Правила получения адреса переданного экземпляра для данного типа
  Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE static PointerType ref(BindingType t) {
    return t;
  }

  /// Правила получения указателя на ссылку переданного экземпляра для данного типа
  Q_INLINE_TEMPLATE static BindingType base(bool &is_valid, BindingType t) {
    is_valid = true;
    return t;
  }

  /// Правила удаления переданного экземпляра для данного типа
  Q_INLINE_TEMPLATE static void destroy(BindingType t) {
    delete t;
    t = Q_NULLPTR;
  }
};

/// Вспомогательный шаблон описывающий правила работы со свойством являющимся значением (ссылкой)
template <typename T>
struct value<T, false> : std::unary_function<T, void> {
  using TM            = TypeManager<T>;
  using BindingType   = typename TM::BindingType;
  using PointerType   = typename TM::PointerType;
  using ReferenceType = typename TM::ReferenceType;

  /// Правила создания экземпляра для данного типа
  Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE static PointerType create() {
    return Q_NULLPTR;
  }

  /// Правила разыменования переданного экземпляра для данного типа
  Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE static ReferenceType deref(BindingType t) {
    return t;
  }

  /// Правила получения адреса переданного экземпляра для данного типа
  Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE static PointerType ref(BindingType t) {
    return &t;
  }

  /// Правила получения указателя на ссылку переданного экземпляра для данного типа
  Q_INLINE_TEMPLATE static typename TM::PointerRefType base(bool &is_valid, BindingType) {
    static PointerType p_null { Q_NULLPTR };
    is_valid = false;
    return p_null;
  }

  /// Правила удаления переданного экземпляра для данного типа
  Q_INLINE_TEMPLATE static void destroy(BindingType) { }
};

} // namespace inner
} // namespace properties
} // namespace netcom
