#include <iostream>
#include <locale>
#include <codecvt>
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

void check(const wstring& text, int key, bool destructCipherText = false)
{
    try {
        Table cipher(key);
        wstring cipherText = cipher.encrypt(text);
        if (destructCipherText && !cipherText.empty()) {
            wstring lower = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
            wstring upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
            size_t pos = upper.find(cipherText.front());
            if (pos != wstring::npos)
                cipherText.front() = lower[pos];
        }
        wstring decryptedText = cipher.decrypt(cipherText);
        cout << "key=" << key << endl;
        cout << w_to_str8(text) << endl;
        cout << w_to_str8(cipherText) << endl;
        cout << w_to_str8(decryptedText) << endl;
        if (text == decryptedText || !destructCipherText)
            cout << "Ok" << endl;
        else
            cout << "Err" << endl;
    } catch (const cipher_error& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    string mode;
    cout << "Режим: test / ui: ";
    getline(cin, mode);

    if (mode == "test") {
        cout << "=== Проверка зашифрования/расшифрования ===" << endl;
        check(L"ПРИВЕТ", 3);
        check(L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ", 4);
        check(L"СЕКРЕТНОЕСООБЩЕНИЕ", 5);

        cout << "\n=== Проверка ключа ===" << endl;
        cout << "-- Ключ = 0:" << endl;
        check(L"ПРИВЕТ", 0);

        cout << "-- Ключ = 1:" << endl;
        check(L"ПРИВЕТ", 1);

        cout << "-- Ключ < 0:" << endl;
        check(L"ПРИВЕТ", -5);

        cout << "\n=== Проверка открытого текста ===" << endl;
        cout << "-- Текст с пробелами и знаками:" << endl;
        check(L"Привет, Мир!", 3);

        cout << "-- Пустой текст:" << endl;
        check(L"", 3);

        cout << "-- Только цифры:" << endl;
        check(L"12345", 3);

        cout << "\n=== Проверка шифротекста ===" << endl;
        cout << "-- Испорченный шифротекст:" << endl;
        check(L"ПРИВЕТ", 3, true);

    } else {
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
                cout << "Выберите режим (0-выход, 1-шифрование, 2-расшифровка): ";
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
                        cerr << "Ошибка: " << e.what() << endl;
                    }
                }
            } while (action != 0);

        } catch (const cipher_error& e) {
            cerr << "Ошибка: " << e.what() << endl;
            return 1;
        } catch (const exception& e) {
            cerr << "Ошибка: " << e.what() << endl;
            return 1;
        }
    }
    return 0;
}
