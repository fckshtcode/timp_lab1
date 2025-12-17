#include "table.h"
#include <algorithm>
#include <vector>
#include <cwctype>  
using namespace std;

int Table::getValidKey(const int key)
{
    if (key <= 0)
        throw cipher_error("Invalid key: key must be positive");
    return key;
}
wstring Table::getValidOpenText(const wstring& s)
{
    wstring tmp;
    wstring lower = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    wstring upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    
    for (auto c : s) {
        if (!iswspace(c)) { 
            if (upper.find(c) != wstring::npos) {
                tmp.push_back(c);
            } else {
    
                size_t pos = lower.find(c);
                if (pos != wstring::npos) {
                    tmp.push_back(upper[pos]); 
                }
            }
        }
    }
    if (tmp.empty())
        throw cipher_error("Empty open text");
    return tmp;
}

wstring Table::getValidCipherText(const wstring& s)
{
    wstring tmp;
    for (auto c : s) {
        if (!iswspace(c)) {
            tmp.push_back(c);
        }
    }
    
    if (tmp.empty())
        throw cipher_error("Empty cipher text");
    
    wstring upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";  
    for (auto c : tmp) {
        if (upper.find(c) == wstring::npos)
            throw cipher_error("Invalid cipher text");
    }
    return tmp;
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

    vector<vector<wchar_t>> grid(rows, vector<wchar_t>(cols, L' '));
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
            if (grid[r][c] != L' ') {
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

    vector<vector<wchar_t>> grid(rows, vector<wchar_t>(cols, L' '));
    int pos = 0;

    for (int c = cols - 1; c >= 0; --c) {
        int h = rows;
    
        if (c >= fullCols) {
            h = rows - 1;
        }
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
            if (grid[r][c] != L' ') {
                out += grid[r][c];
            }
        }
    }
    return out;
}
