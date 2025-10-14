#include <iostream>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <cwctype>
#include <limits>
#include "modAlphaCipher.h"

using namespace std;
{
    wstring ABC = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    for (auto ch : s) {
        if (iswspace(ch)) continue;
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
        if (pos != wstring::npos)
            c = upper[pos];
    }
    return res;
}

wstring stripSpaces(const wstring& s)
{
    wstring out;
    out.reserve(s.size());
    for (wchar_t ch : s)
        if (!iswspace(ch))
            out.push_back(ch);
    return out;
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
    wstring keyW = toUpperCyr(str8_to_w(keyLine));

    if (!onlyCyrillic(keyW)) {
        cerr << "Некорректный ключ: допускаются лишь русские буквы." << endl;
        return 1;
    }
    cout << "Ключ подходит." << endl;

    modAlphaCipher cipher(keyW);

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

            wstring msgW = toUpperCyr(str8_to_w(msgLine));
            wstring clean = stripSpaces(msgW);  

            if (onlyCyrillic(clean)) {
                if (action == 1) {
                    wstring enc = cipher.encrypt(clean);
                    cout << "Зашифровано: " << w_to_str8(enc) << endl;
                } else {
                    wstring dec = cipher.decrypt(clean);
                    cout << "Расшифровано: " << w_to_str8(dec) << endl;
                }
            } else {
                cout << "Некорректный ввод: используйте кириллицу." << endl;
            }
        }
    } while (action != 0);

    return 0;
}
