#include "table.h"
#include <algorithm>
using namespace std;

Table::Table(int key)
{
    if (key <= 0) throw invalid_argument("Ключ должен быть > 0");
    cols = key;
}

wstring Table::encrypt(const wstring& plain)
{
    if (plain.empty()) throw invalid_argument("Текст не может быть пустым");

    int n = static_cast<int>(plain.length());
    int rows = (n + cols - 1) / cols;
    vector<vector<wchar_t>> grid(rows, vector<wchar_t>(cols, L' '));
    int pos = 0;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (pos < n) grid[r][c] = plain[pos++];
            else grid[r][c] = L'Я';
        }
    }

    wstring out;
    for (int c = cols - 1; c >= 0; --c) {
        for (int r = 0; r < rows; ++r) {
            out += grid[r][c];
        }
    }
    return out;
}

wstring Table::decrypt(const wstring& cipher)
{
    if (cipher.empty()) throw invalid_argument("Текст не может быть пустым");

    int total = static_cast<int>(cipher.length());
    int rows = total / cols;
    if (total % cols != 0) throw invalid_argument("Неправильная длина зашифрованного текста");

    vector<vector<wchar_t>> grid(rows, vector<wchar_t>(cols, L' '));
    int pos = 0;

    for (int c = cols - 1; c >= 0; --c) {
        for (int r = 0; r < rows; ++r) {
            grid[r][c] = cipher[pos++];
        }
    }

    wstring out;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            out += grid[r][c];
        }
    }
    while (!out.empty() && out.back() == L'Я') out.pop_back();
    return out;
}