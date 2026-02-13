/** @file main.cpp
 * @author Ладыгин П.А.
 * @version 1.0
 * @date 17.12.25
 * @brief Главный модуль программы шифрования методом Гронсфельда
 */
#include <iostream>
#include <locale>
#include <codecvt>
#include <limits>
#include "modAlphaCipher.h"

using namespace std;

/** @brief Преобразует строку UTF-8 в wstring
 * @param s Строка в кодировке UTF-8
 * @return Широкая строка
 */
wstring str8_to_w(const string& s)
{
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(s);
}

/** @brief Преобразует wstring в строку UTF-8
 * @param ws Широкая строка
 * @return Строка в кодировке UTF-8
 */
string w_to_str8(const wstring& ws)
{
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(ws);
}

/** @brief Точка входа в программу
 * @return 0 при успехе, 1 при ошибке инициализации
 */
int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    string keyLine;
    string msgLine;
    unsigned action;

    cout << "Введите ключ: ";
    getline(cin, keyLine);

    try {
        modAlphaCipher cipher(str8_to_w(keyLine));
        cout << "Ключ загружен." << endl;

        do {
            cout << "Выберите режим (0-выход, 1-шифрование, 2-расшифровка): ";
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
                        wstring enc = cipher.encrypt(str8_to_w(msgLine));
                        cout << "Зашифровано: " << w_to_str8(enc) << endl;
                    } else {
                        wstring dec = cipher.decrypt(str8_to_w(msgLine));
                        cout << "Расшифровано: " << w_to_str8(dec) << endl;
                    }
                } catch (const cipher_error& e) {
                    cerr << "Ошибка: " << e.what() << endl;
                }
            }
        } while (action != 0);

    } catch (const cipher_error& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}
