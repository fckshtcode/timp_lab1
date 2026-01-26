/**
 * @file main.cpp
 * @brief Основная программа для работы с шифром модифицированного алфавитного шифра
 * @author Ладыгин П.А.
 * @date 17.12.25
 * @version 1.0
 * 
 * Программа предоставляет консольный интерфейс для шифрования и расшифровки текста
 * с использованием модифицированного алфавитного шифра.
 */

#include <iostream>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <cwctype>
#include <limits>
#include "modAlphaCipher.h"

using namespace std;

/**
 * @brief Преобразует строку UTF-8 в широкую строку (wstring)
 * @param s Строка в кодировке UTF-8
 * @return Широкая строка (wstring)
 */
wstring str8_to_w(const string& s)
{
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(s);
}

/**
 * @brief Преобразует широкую строку (wstring) в строку UTF-8
 * @param ws Широкая строка (wstring)
 * @return Строка в кодировке UTF-8
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
 * Функция предоставляет пользовательский интерфейс для:
 * 1. Ввода ключа шифрования
 * 2. Выбора режима работы (шифрование/расшифровка)
 * 3. Ввода текста для обработки
 * 4. Вывода результатов
 */
int main()
{
    // Установка локали для поддержки кириллицы
    setlocale(LC_ALL, "ru_RU.UTF-8");

    string keyLine;   ///< Строка для хранения введенного ключа
    string msgLine;   ///< Строка для хранения введенного текста
    unsigned action;  ///< Переменная для выбора действия пользователем

    // Ввод ключа шифрования
    cout << "Введите ключ: ";
    getline(cin, keyLine);

    try {
        // Создание объекта шифра с введенным ключом
        modAlphaCipher cipher(str8_to_w(keyLine));
        cout << "Ключ загружен." << endl;

        // Основной цикл программы
        do {
            cout << "Выберите режим (0 — выход, 1 — шифрование, 2 — расшифровка): ";
            if (!(cin >> action))
                return 0;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (action > 2) {
                cout << "Неверный выбор режима." << endl;
            } else if (action > 0) {
                cout << "Введите строку: ";
                getline(cin, msgLine);

                try {
                    if (action == 1) {
                        // Шифрование введенного текста
                        wstring enc = cipher.encrypt(str8_to_w(msgLine));
                        cout << "Зашифровано: " << w_to_str8(enc) << endl;
                    } else {
                        // Расшифровка введенного текста
                        wstring dec = cipher.decrypt(str8_to_w(msgLine));
                        cout << "Расшифровано: " << w_to_str8(dec) << endl;
                    }
                } catch (const cipher_error& e) {
                    // Обработка ошибок при шифровании/расшифровке
                    cerr << "Ошибка при обработке текста: " << e.what() << endl;
                }
            }
        } while (action != 0);
        
    } catch (const cipher_error& e) {
        // Обработка ошибок инициализации шифра
        cerr << "Ошибка инициализации шифра: " << e.what() << endl;
        return 1;
    }

    return 0;
}
