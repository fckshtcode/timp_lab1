/** @file modAlphaCipher.h
 * @author Ладыгин П.А.
 * @version 1.0
 * @date 17.12.25
 * @brief Заголовочный файл модуля шифрования методом Гронсфельда
 */
#pragma once
#include <vector>
#include <string>
#include <map>
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

/** @brief Шифрование методом Гронсфельда
 * @details Ключ устанавливается в конструкторе.
 * Для зашифровывания и расшифровывания предназначены методы encrypt и decrypt.
 * @warning Реализация только для русского языка
 */
class modAlphaCipher
{
private:
    std::wstring alphabet = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"; ///< алфавит по порядку
    std::map<wchar_t, int> alphaIndex; ///< ассоциативный массив "номер по символу"
    std::vector<int> keySeq; ///< ключ в числовом виде
    /** @brief Преобразование строки в числовой вектор
     * @param s Входная строка
     * @return Вектор числовых индексов символов
     */
    std::vector<int> toNums(const std::wstring& s);
    /** @brief Преобразование числового вектора в строку
     * @param v Вектор числовых индексов
     * @return Строка символов алфавита
     */
    std::wstring toStr(const std::vector<int>& v);
    /** @brief Валидация и нормализация ключа
     * @param s Входной ключ
     * @return Валидный ключ в верхнем регистре
     * @throw cipher_error если ключ пустой, содержит недопустимые символы или вырожденный
     */
    std::wstring getValidKey(const std::wstring& s);
    /** @brief Валидация и нормализация открытого текста
     * @param [in] s Открытый текст. Строчные символы преобразуются к прописным. Все не-буквы удаляются
     * @return Валидный открытый текст
     * @throw cipher_error если текст пустой после очистки
     */
    std::wstring getValidOpenText(const std::wstring& s);
    /** @brief Валидация шифротекста
     * @param [in] s Шифротекст. Должен содержать только прописные русские буквы
     * @return Валидный шифротекст
     * @throw cipher_error если текст пустой или содержит недопустимые символы
     */
    std::wstring getValidCipherText(const std::wstring& s);

public:
    modAlphaCipher() = delete; ///< запрет конструктора без параметров
    /** @brief Конструктор для установки ключа
     * @param keyStr Ключ шифрования в виде строки
     * @throw cipher_error если ключ невалидный
     */
    modAlphaCipher(const std::wstring& keyStr);
    /** @brief Зашифровывание
     * @param [in] plain Открытый текст
     * @return Зашифрованная строка
     * @throw cipher_error если текст пустой после очистки
     */
    std::wstring encrypt(const std::wstring& plain);
    /** @brief Расшифровывание
     * @param [in] cipher Шифротекст
     * @return Расшифрованная строка
     * @throw cipher_error если шифротекст невалидный
     */
    std::wstring decrypt(const std::wstring& cipher);
};
