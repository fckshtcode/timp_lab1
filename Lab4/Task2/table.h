/** @file table.h
 * @author Ладыгин П.А.
 * @version 1.0
 * @date 17.12.25
 * @brief Заголовочный файл класса табличного шифра маршрутной перестановки
 * @details Определяет класс cipher_error для обработки ошибок шифрования
 * и класс Table для реализации табличного шифра.
 */
#pragma once
#include <string>
#include <vector>
#include <stdexcept>

/** @brief Класс исключений для ошибок шифрования
 * @details Наследуется от std::invalid_argument.
 * Используется для передачи информации об ошибках при работе с шифром.
 */
class cipher_error: public std::invalid_argument {
public:
    /** @brief Конструктор с параметром типа string
     * @param what_arg Сообщение об ошибке
     */
    explicit cipher_error (const std::string& what_arg):
        std::invalid_argument(what_arg) {}
    /** @brief Конструктор с параметром типа const char*
     * @param what_arg Сообщение об ошибке
     */
    explicit cipher_error (const char* what_arg):
        std::invalid_argument(what_arg) {}
};

/** @brief Шифрование методом маршрутной перестановки (табличный шифр)
 * @details Текст записывается в таблицу по строкам слева направо,
 * а считывается по столбцам справа налево. Ключ — количество столбцов.
 * @warning Реализация только для русского языка
 */
class Table
{
private:
    int cols; ///< количество столбцов в таблице (ключ)
    /** @brief Валидация ключа
     * @param key Количество столбцов
     * @return Валидный ключ
     * @throws cipher_error если ключ <= 1
     */
    int getValidKey(int key);
    /** @brief Валидация и нормализация открытого текста
     * @param s Открытый текст
     * @return Текст в верхнем регистре, только русские буквы
     * @throws cipher_error если текст пустой после очистки
     */
    std::wstring getValidOpenText(const std::wstring& s);
    /** @brief Валидация шифротекста
     * @param s Шифротекст
     * @return Валидный шифротекст
     * @throws cipher_error если текст пустой или содержит недопустимые символы
     */
    std::wstring getValidCipherText(const std::wstring& s);

public:
    Table() = delete; ///< запрет конструктора без параметров
    /** @brief Конструктор для установки ключа
     * @param key Количество столбцов (ключ шифрования)
     * @throws cipher_error если ключ невалидный
     */
    explicit Table(int key);
    /** @brief Зашифровывание
     * @param plain Открытый текст
     * @return Зашифрованная строка
     * @throws cipher_error если текст невалидный
     */
    std::wstring encrypt(const std::wstring& plain);
    /** @brief Расшифровывание
     * @param cipher Шифротекст
     * @return Расшифрованная строка
     * @throws cipher_error если шифротекст невалидный
     */
    std::wstring decrypt(const std::wstring& cipher);
};
