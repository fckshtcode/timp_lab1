#include <iostream>
#include <locale>
#include <codecvt>
#include "table.h"
using namespace std;

bool onlyCyrillic(const wstring& s)
{
    wstring ABC = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    for (auto ch : s) {
        if (ABC.find(ch) == wstring::npos) return false;
    }
    return true;
}

wstring toUpperCyr(const wstring& s)
{
    wstring res = s;
    wstring lower = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    wstring upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    for (auto& c : res) {
        size_t pos = lower.find(c);
        if (pos != wstring::npos) c = upper[pos];
    }
    return res;
}

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

    int cols = 0;
    try {
        cols = stoi(keyLine);
    } catch (...) {
        cerr << "Некорректное число столбцов." << endl;
        return 1;
    }
    if (cols <= 0) {
        cerr << "Число столбцов должно быть положительным." << endl;
        return 1;
    }

    Table cipher(cols);
    cout << "Режимы: 1 — шифрование, 2 — расшифровка, 0 — выход." << endl;

    do {
        cout << "Выберите режим (0 — выход, 1 — шифрование, 2 — расшифровка): ";
        if (!(cin >> action)) return 0;
        cin.ignore();

        if (action > 2) {
            cout << "Неверный выбор режима." << endl;
        } else if (action > 0) {
            cout << "Введите строку: ";
            getline(cin, msgLine);
            wstring msgW = str8_to_w(msgLine);
            msgW = toUpperCyr(msgW);

            if (onlyCyrillic(msgW)) {
                if (action == 1) {
                    wstring enc = cipher.encrypt(msgW);
                    cout << "Зашифровано: " << w_to_str8(enc) << endl;
                } else {
                    wstring dec = cipher.decrypt(msgW);
                    cout << "Расшифровано: " << w_to_str8(dec) << endl;
                }
            } else {
                cout << "Можно использовать только кириллицу." << endl;
            }
        }
    } while (action != 0);

    return 0;
}