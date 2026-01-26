/**
 * @file modAlphaCipher.h
 * @brief Заголовочный файл класса модифицированного алфавитного шифра
 * @author Ладыгин П.А.
 * @date 17.12.25
 * @version 1.0
 * 
 * Определяет класс cipher_error для обработки ошибок шифрования
 * и класс modAlphaCipher для реализации шифра.
 */

#ifndef MODALPHACIPHER_H
#define MODALPHACIPHER_H

#include <vector>
#include <string>
#include <map>
#include <locale>
#include <codecvt>
#include <stdexcept>
using namespace std;

/**
 * @class cipher_error
 * @brief Класс для обработки ошибок шифрования
 * 
 * Наследуется от std::invalid_argument, используется для передачи
 * информации об ошибках, возникающих при работе с шифром.
 */
class cipher_error: public invalid_argument {
public:
    /**
     * @brief Конструктор с параметром типа string
     * @param what_arg Сообщение об ошибке
     */
    explicit cipher_error (const string& what_arg):
        invalid_argument(what_arg) {}
    
    /**
     * @brief Конструктор с параметром типа const char*
     * @param what_arg Сообщение об ошибке
     */
    explicit cipher_error (const char* what_arg):
        invalid_argument(what_arg) {}
};

/**
 * @class modAlphaCipher
 * @brief Класс для шифрования и расшифровки текста модифицированным алфавитным шифром
 * 
 * Реализует шифр, использующий русский алфавит и ключ для преобразования текста.
 * Шифр работает по принципу сложения/вычитания числовых значений символов.
 */
class modAlphaCipher
{
private:
    wstring alphabet = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"; ///< Русский алфавит в верхнем регистре
    map<wchar_t, int> alphaIndex; ///< Карта для быстрого доступа к индексам символов
    vector<int> keySeq; ///< Числовая последовательность ключа
    
    /**
     * @brief Преобразует строку в последовательность чисел
     * @param s Входная строка
     * @return Вектор числовых индексов символов
     */
    vector<int> toNums(const wstring& s);
    
    /**
     * @brief Преобразует последовательность чисел в строку
     * @param v Вектор числовых индексов
     * @return Строка, соответствующая индексам
     */
    wstring toStr(const vector<int>& v);
    
    /**
     * @brief Проверяет и нормализует ключ
     * @param s Входной ключ
     * @return Валидный ключ
     * @throws cipher_error если ключ пустой, содержит пробелы или недопустимые символы
     */
    wstring getValidKey(const wstring& s);
    
    /**
     * @brief Проверяет и нормализует открытый текст
     * @param s Входной открытый текст
     * @return Валидный открытый текст
     * @throws cipher_error если текст пустой или содержит недопустимые символы
     */
    wstring getValidOpenText(const wstring& s);
    
    /**
     * @brief Проверяет и нормализует шифртекст
     * @param s Входной шифртекст
     * @return Валидный шифртекст
     * @throws cipher_error если текст пустой, содержит пробелы или недопустимые символы
     */
    wstring getValidCipherText(const wstring& s);
    
public:
    /**
     * @brief Удаленный конструктор по умолчанию
     * 
     * Класс требует ключ для инициализации, поэтому конструктор по умолчанию удален.
     */
    modAlphaCipher() = delete;
    
    /**
     * @brief Конструктор с параметром
     * @param keyStr Ключ шифрования в виде строки
     * @throws cipher_error если ключ невалидный
     */
    modAlphaCipher(const wstring& keyStr);
    
    /**
     * @brief Шифрует открытый текст
     * @param plain Открытый текст для шифрования
     * @return Зашифрованный текст
     * @throws cipher_error если текст невалидный
     */
    wstring encrypt(const wstring& plain);
    
    /**
     * @brief Расшифровывает шифртекст
     * @param cipher Шифртекст для расшифровки
     * @return Расшифрованный текст
     * @throws cipher_error если текст невалидный
     */
    wstring decrypt(const wstring& cipher);
};

#endif // MODALPHACIPHER_H
