/**
 * @file table.h
 * @brief Заголовочный файл класса табличного шифра
 * @author Ладыгин П.А.
 * @date 17.12.25
 * @version 1.0
 * 
 * Определяет класс cipher_error для обработки ошибок шифрования
 * и класс Table для реализации табличного шифра (маршрутной перестановки).
 * Шифр использует таблицу с фиксированным количеством столбцов для шифрования текста.
 */

#pragma once
#include <string>
#include <vector>
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
 * @class Table
 * @brief Класс для шифрования и расшифровки текста табличным методом
 * 
 * Реализует шифр маршрутной перестановки, где текст записывается в таблицу
 * по строкам, а считывается по столбцам (или наоборот).
 * 
 * @note Шифрование: текст записывается в таблицу по строкам, считывается по столбцам
 * @note Расшифровка: текст записывается в таблицу по столбцам, считывается по строкам
 */
class Table
{
private:
    int cols; ///< Количество столбцов в таблице
    
    /**
     * @brief Проверяет валидность ключа (количества столбцов)
     * @param key Количество столбцов для проверки
     * @return Валидное количество столбцов
     * @throws cipher_error если ключ меньше или равен 1
     */
    int getValidKey(const int key);
    
    /**
     * @brief Проверяет и нормализует открытый текст
     * @param s Входной открытый текст
     * @return Валидный открытый текст
     * @throws cipher_error если текст пустой
     */
    wstring getValidOpenText(const wstring& s);
    
    /**
     * @brief Проверяет и нормализует шифртекст
     * @param s Входной шифртекст
     * @return Валидный шифртекст
     * @throws cipher_error если текст пустой
     */
    wstring getValidCipherText(const wstring& s);
    
public:
    /**
     * @brief Конструктор класса Table
     * @param key Количество столбцов в таблице
     * @throws cipher_error если ключ невалидный
     * 
     * Создает объект шифра с заданным количеством столбцов.
     * Количество столбцов должно быть больше 1.
     */
    explicit Table(int key);
    
    /**
     * @brief Шифрует открытый текст
     * @param plain Открытый текст для шифрования
     * @return Зашифрованный текст
     * @throws cipher_error если текст невалидный
     * 
     * Алгоритм шифрования:
     * 1. Текст записывается в таблицу по строкам
     * 2. Текст считывается из таблицы по столбцам сверху вниз
     */
    wstring encrypt(const wstring& plain);
    
    /**
     * @brief Расшифровывает шифртекст
     * @param cipher Шифртекст для расшифровки
     * @return Расшифрованный текст
     * @throws cipher_error если текст невалидный
     * 
     * Алгоритм расшифровки:
     * 1. Текст записывается в таблицу по столбцам
     * 2. Текст считывается из таблицы по строкам слева направо
     */
    wstring decrypt(const wstring& cipher);
};
