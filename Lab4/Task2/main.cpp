/**
 * @file main.cpp
 * @brief Основная программа для работы с табличным шифром
 * @author Ладыгин П.А.
 * @date 17.12.25
 * @version 1.0
 * 
 * Программа предоставляет консольный интерфейс для шифрования и расшифровки текста
 * с использованием табличного шифра (маршрутной перестановки).
 */

#include <iostream>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <cwctype>
#include <limits>
#include <string>
#include "table.h"
using namespace std;

/**
 * @brief Преобразует строку UTF-8 в широкую строку (wstring)
 * @param s Строка в кодировке UTF-8
 * @return Широкая строка (wstring) в Unicode
 * 
 * Используется для корректной работы с русским текстом в программе.
 */
wstring str8_to_w(const string& s)
{
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(s);
}

/**
 * @brief Преобразует широкую строку (wstring) в строку UTF-8
 * @param ws Широкая строка (wstring) в Unicode
 * @return Строка в кодировке UTF-8
 * 
 * Используется для вывода текста в консоль.
 */
string w_to_str8(const wstring& ws)
{
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(ws);
}

/**
 * @brief Основная функция программы
 * @return Код завершения программы (0 - успешно, 1 - ошибка)
 * 
 * Программа выполняет следующие действия:
 * 1. Запрашивает количество столбцов для таблицы
 * 2. Создает объект шифра Table с указанным количеством столбцов
 * 3. Предоставляет меню для выбора операций (шифрование, расшифровка, выход)
 * 4. Обрабатывает ввод пользователя и выводит результаты
 * 
 * @note Для корректной работы с русским языком установлена локаль ru_RU.UTF-8
 */
int main()
{
    // Установка локали для поддержки кириллицы
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    string keyLine;   ///< Строка для ввода количества столбцов
    string msgLine;   ///< Строка для ввода текста
    unsigned action;  ///< Переменная для выбора действия

    // Ввод количества столбцов таблицы
    cout << "Введите число столбцов: ";
    getline(cin, keyLine);

    try {
        // Преобразование строки в число и создание объекта шифра
        int cols = stoi(keyLine);
        Table cipher(cols);
        cout << "Таблица создана." << endl;

        // Основной цикл программы
        do {
            cout << "Выберите режим (0 — выход, 1 — шифрование, 2 — расшифровка): ";
            if (!(cin >> action)) {
                // Очистка потока ввода при ошибке
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (action > 2) {
                cout << "Неверный выбор режима." << endl;
            } else if (action > 0) {
                // Ввод текста для обработки
                cout << "Введите строку: ";
                getline(cin, msgLine);

                try {
                    if (action == 1) {
                        // Шифрование текста
                        wstring enc = cipher.encrypt(str8_to_w(msgLine));
                        cout << "Зашифровано: " << w_to_str8(enc) << endl;
                    } else {
                        // Расшифровка текста
                        wstring dec = cipher.decrypt(str8_to_w(msgLine));
                        cout << "Расшифровано: " << w_to_str8(dec) << endl;
                    }
                } catch (const cipher_error& e) {
                    // Обработка ошибок шифрования/расшифровки
                    cerr << "Ошибка при обработке текста: " << e.what() << endl;
                }
            }
        } while (action != 0);
        
    } catch (const cipher_error& e) {
        // Обработка ошибок инициализации шифра
        cerr << "Ошибка инициализации шифра: " << e.what() << endl;
        return 1;
    } catch (const exception& e) {
        // Обработка общих исключений (например, stoi)
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }
    return 0;
}
