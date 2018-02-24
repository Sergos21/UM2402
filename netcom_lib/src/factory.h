/*! ***************************************************************************
* Проект: src.pro
*
* @file factory.h
*
* @brief: Заголовочный файл фабрики контрактов, необходимой для десериализации
* и проверки уникальности имен контрактов во всех единицах трансляции проекта
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#pragma once

#include <QHash>
#include <QMetaObject>
#include <QScopedPointer>
#include <functional>

#include "contract.h"

namespace netcom {

/// Вариативный шаблон реализующий паттерн AbstractFactory с поддержкой множества различных конструкторов
template <typename ...Args>
class BaseFactory {
public:
  /// Описание функции-конструктора
  using Ctor = std::function<Contract*(Args...)>;

  /// Регистрация контракта по имени и аргументам конструктора контракта
  static bool registerContract(const QString &name, const Ctor &ctor, Args&&... args);

  /// Создание экземпляра контракта по его уникальному имени
  static Contract *createContractInstance(const QString &name, Args&&... args);

private:
  using Hash = QHash<QString, Ctor>;
  static Hash &getHash();
};

/// Принудительная специализация шаблона, необходимая для предотвращения создания отдельного экземпляра
/// шаблона в другом адресном пространстве поверх кода самой библиотеки
extern template class NETCOM_SHARED_EXPORT BaseFactory<>;
using Factory = BaseFactory<>;

/// Вспомогательная функция для упрощенной регистрации контракта
template <typename T, typename ...Args>
Q_OUTOFLINE_TEMPLATE bool registerContract(const QString &name, Args&&... args) {
  auto lambda = [&, args...](Args...) -> Contract* {
    return new T(std::forward<Args>(args)...);
  };
  return Factory::registerContract(name, static_cast<const typename Factory::Ctor&>(lambda), args...);
}

} // namespace netcom
