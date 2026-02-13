#include "table.h"
#include <vector>
using namespace std;

int Table::getValidKey(int key)
{
    if (key <= 1)
        throw cipher_error("Недопустимый ключ: должен быть > 1");
    return key;
}

wstring Table::getValidOpenText(const wstring& s)
{
    wstring tmp;
    wstring lower = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    wstring upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

    for (auto c : s) {
        if (upper.find(c) != wstring::npos) {
            tmp.push_back(c);
        } else {
            size_t pos = lower.find(c);
            if (pos != wstring::npos) {
                tmp.push_back(upper[pos]);
            }
        }
    }
    if (tmp.empty())
        throw cipher_error("Пустой открытый текст");
    return tmp;
}

wstring Table::getValidCipherText(const wstring& s)
{
    if (s.empty())
        throw cipher_error("Пустой шифротекст");

    wstring upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    for (auto c : s) {
        if (upper.find(c) == wstring::npos)
            throw cipher_error("Недопустимый шифротекст");
    }
    return s;
}

Table::Table(int key)
{
    cols = getValidKey(key);
}

wstring Table::encrypt(const wstring& plain)
{
    wstring validText = getValidOpenText(plain);
    int n = static_cast<int>(validText.length());
    int rows = (n + cols - 1) / cols;

    vector<vector<wchar_t>> grid(rows, vector<wchar_t>(cols, L'\0'));
    int pos = 0;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (pos < n) {
                grid[r][c] = validText[pos++];
            }
        }
    }

    wstring out;
    out.reserve(n);
    for (int c = cols - 1; c >= 0; --c) {
        for (int r = 0; r < rows; ++r) {
            if (grid[r][c] != L'\0') {
                out += grid[r][c];
            }
        }
    }
    return out;
}

wstring Table::decrypt(const wstring& cipher)
{
    wstring validText = getValidCipherText(cipher);
    int n = static_cast<int>(validText.length());
    int rows = (n + cols - 1) / cols;

    int fullCols = n % cols;
    if (fullCols == 0) fullCols = cols;

    vector<vector<wchar_t>> grid(rows, vector<wchar_t>(cols, L'\0'));
    int pos = 0;

    for (int c = cols - 1; c >= 0; --c) {
        int h = (c < fullCols) ? rows : rows - 1;
        for (int r = 0; r < h; ++r) {
            if (pos < n) {
                grid[r][c] = validText[pos++];
            }
        }
    }

    wstring out;
    out.reserve(n);
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] != L'\0') {
                out += grid[r][c];
            }
        }
    }
    return out;
}
