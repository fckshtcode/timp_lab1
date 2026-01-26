/**
 * @file table.cpp
 * @brief Реализация класса табличного шифра
 * @author Ладыгин П.А.
 * @date 17.12.25
 * @version 1.0
 * 
 * Содержит реализацию методов класса Table для шифрования и расшифровки текста
 * методом маршрутной перестановки (табличного шифра).
 */

#include "table.h"
#include <cwctype>
#include <algorithm>
using namespace std;

/**
 * @brief Проверяет валидность ключа (количества столбцов)
 * @param key Количество столбцов для проверки
 * @return Валидное количество столбцов
 * @throws cipher_error если ключ меньше или равен 1
 * 
 * Для корректной работы табличного шифра необходимо, чтобы количество столбцов
 * было больше 1. В противном случае шифрование теряет смысл.
 */
int Table::getValidKey(const int key)
{
    if (key <= 1) {
        throw cipher_error("Key must be greater than 1");
    }
    return key;
}

/**
 * @brief Проверяет и нормализует открытый текст
 * @param s Входной открытый текст
 * @return Валидный открытый текст
 * @throws cipher_error если текст пустой
 * 
 * Удаляет все пробелы из текста, так как табличный шифр работает
 * только с непрерывной последовательностью символов.
 * Все буквы преобразуются к верхнему регистру.
 */
wstring Table::getValidOpenText(const wstring& s)
{
    wstring tmp;
    
    // Удаление пробелов и преобразование к верхнему регистру
    for (auto c : s) {
        if (!iswspace(c)) {
            // Преобразование к верхнему регистру
            if (c >= L'а' && c <= L'я') {
                tmp.push_back(c - L'а' + L'А');
            } else if (c == L'ё') {
                tmp.push_back(L'Ё');
            } else {
                tmp.push_back(c);
            }
        }
    }
    
    if (tmp.empty())
        throw cipher_error("Empty open text");
        
    return tmp;
}

/**
 * @brief Проверяет и нормализует шифртекст
 * @param s Входной шифртекст
 * @return Валидный шифртекст
 * @throws cipher_error если текст пустой
 * 
 * Проверяет, что шифртекст не содержит пробелов,
 * так как табличный шифр генерирует текст без пробелов.
 */
wstring Table::getValidCipherText(const wstring& s)
{
    wstring tmp;
    
    // Проверка на наличие пробелов
    for (auto c : s) {
        if (iswspace(c)) {
            throw cipher_error("Cipher text cannot contain spaces");
        }
        tmp.push_back(c);
    }
    
    if (tmp.empty())
        throw cipher_error("Empty cipher text");
    
    return tmp;
}

/**
 * @brief Конструктор класса Table
 * @param key Количество столбцов в таблице
 * @throws cipher_error если ключ невалидный
 * 
 * Инициализирует объект шифра с заданным количеством столбцов.
 * Количество строк вычисляется динамически в зависимости от длины текста.
 */
Table::Table(int key)
{
    cols = getValidKey(key);
}

/**
 * @brief Шифрует открытый текст
 * @param plain Открытый текст для шифрования
 * @return Зашифрованный текст
 * @throws cipher_error если текст невалидный
 * 
 * Алгоритм шифрования:
 * 1. Валидация и нормализация текста
 * 2. Вычисление количества строк: rows = ceil(text.length() / cols)
 * 3. Создание таблицы размером rows x cols
 * 4. Запись текста в таблицу по строкам (слева направо)
 * 5. Считывание текста из таблицы по столбцам (сверху вниз)
 * 6. Пустые ячейки заполняются символом '_'
 */
wstring Table::encrypt(const wstring& plain)
{
    wstring validText = getValidOpenText(plain);
    int len = validText.length();
    
    // Вычисление количества строк
    int rows = (len + cols - 1) / cols;  // Округление вверх
    
    // Создание таблицы
    vector<vector<wchar_t>> table(rows, vector<wchar_t>(cols, L'_'));
    
    // Заполнение таблицы по строкам
    int index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (index < len) {
                table[i][j] = validText[index++];
            } else {
                table[i][j] = L'_';  // Заполнитель для пустых ячеек
            }
        }
    }
    
    // Считывание по столбцам
    wstring result;
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            result.push_back(table[i][j]);
        }
    }
    
    return result;
}

/**
 * @brief Расшифровывает шифртекст
 * @param cipher Шифртекст для расшифровки
 * @return Расшифрованный текст
 * @throws cipher_error если текст невалидный
 * 
 * Алгоритм расшифровки:
 * 1. Валидация шифртекста
 * 2. Вычисление количества строк: rows = text.length() / cols
 * 3. Создание таблицы размером rows x cols
 * 4. Запись текста в таблицу по столбцам (сверху вниз)
 * 5. Считывание текста из таблицы по строкам (слева направо)
 * 6. Удаление символов-заполнителей '_' из результата
 */
wstring Table::decrypt(const wstring& cipher)
{
    wstring validText = getValidCipherText(cipher);
    int len = validText.length();
    
    // Вычисление количества строк
    int rows = len / cols;
    if (rows * cols != len) {
        throw cipher_error("Invalid cipher text length");
    }
    
    // Создание таблицы
    vector<vector<wchar_t>> table(rows, vector<wchar_t>(cols));
    
    // Заполнение таблицы по столбцам
    int index = 0;
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            table[i][j] = validText[index++];
        }
    }
    
    // Считывание по строкам и удаление заполнителей
    wstring result;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (table[i][j] != L'_') {
                result.push_back(table[i][j]);
            }
        }
    }
    
    return result;
}
