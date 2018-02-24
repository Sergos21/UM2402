/*! ***************************************************************************
* Проект: src.pro
*
* @file serialization/json.h
*
* @brief: Заголовочный файл класса сериализатора для JSON-формата
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#pragma once

#include <QByteArray>

#include "src/common.h"

namespace netcom {

class Contract;

namespace serialization {

/// <summary> Сериализовать контракт в строку формата JSON </summary>
/// <param name="contract"> Указатель на сериализуемый контракт </param>
/// <param name="ok"> Результат преобразования (true/false) </param>
/// <returns> QByteArray </returns>
NETCOM_SHARED_EXPORT QByteArray ToJSON(const Contract *contract, bool *ok = Q_NULLPTR);

/// <summary> Десериализовать контракт из строки формата JSON </summary>
/// <param name="data"> Массив данных для десериализации </param>
/// <param name="ok"> Результат преобразования (true/false) </param>
/// <returns>
/// Созданный экземпляр Contract*, который должен быть удален пользователем!
///  </returns>
NETCOM_SHARED_EXPORT Contract *FromJSON(const QByteArray &data, bool *ok = Q_NULLPTR);

/// <summary> Попытка десериализовать данные в уже созданный контракт из строки формата JSON </summary>
/// <param name="contract"> Указатель на контракт подлежащий десериализации </param>
/// <param name="data"> Массив данных для десериализации </param>
/// <returns> Результат выполнения (true/fasle) </returns>
NETCOM_SHARED_EXPORT bool FromJSON(Contract *contract, const QByteArray &data);

} // namespace serialization
} // namespace netcom
