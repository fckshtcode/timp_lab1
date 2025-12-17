#include <iostream>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <cwctype>
#include <limits>
#include <string>
#include "table.h"
using namespace std;

wstring str8_to_w(const string& s)
{
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(s);
}

string w_to_str8(const wstring& ws)
{
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(ws);
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    string keyLine;
    string msgLine;
    unsigned action;

    cout << "Введите число столбцов: ";
    getline(cin, keyLine);

    try {
        int cols = stoi(keyLine);
        Table cipher(cols);
        cout << "Таблица создана." << endl;

        do {
            cout << "Выберите режим (0 — выход, 1 — шифрование, 2 — расшифровка): ";
            if (!(cin >> action)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
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
                    cerr << "Ошибка при обработке текста: " << e.what() << endl;
                }
            }
        } while (action != 0);
        
    } catch (const cipher_error& e) {
        cerr << "Ошибка инициализации шифра: " << e.what() << endl;
        return 1;
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }
    return 0;
}
