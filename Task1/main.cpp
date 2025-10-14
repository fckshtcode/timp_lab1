#include <iostream>
#include <locale>
#include <codecvt>
#include "modAlphaCipher.h"
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

    cout << "Введите ключ: ";
    getline(cin, keyLine);
    wstring keyW = str8_to_w(keyLine);
    keyW = toUpperCyr(keyW);

    if (!onlyCyrillic(keyW)) {
        cerr << "Некорректный ключ: допускаются лишь русские буквы." << endl;
        return 1;
    }
    cout << "Ключ подходит." << endl;

    modAlphaCipher cipher(keyW);

    do {
        cout << "Выберите режим (0 — выход, 1 — шифрование, 2 — расшифровка): ";
        cin >> action;
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
                cout << "Некорректный ввод: используйте кириллицу." << endl;
            }
        }
    } while (action != 0);

    return 0;
}