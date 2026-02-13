#include "modAlphaCipher.h"
using namespace std;

modAlphaCipher::modAlphaCipher(const wstring& keyStr)
{
    for (unsigned k = 0; k < alphabet.size(); ++k) {
        alphaIndex[alphabet[k]] = k;
    }
    keySeq = toNums(getValidKey(keyStr));
}

vector<int> modAlphaCipher::toNums(const wstring& s)
{
    vector<int> resultNums;
    resultNums.reserve(s.size());
    for (auto sym : s) {
        resultNums.push_back(alphaIndex[sym]);
    }
    return resultNums;
}

wstring modAlphaCipher::toStr(const vector<int>& v)
{
    wstring resultStr;
    resultStr.reserve(v.size());
    for (auto idx : v) {
        resultStr.push_back(alphabet[idx]);
    }
    return resultStr;
}

wstring modAlphaCipher::getValidKey(const wstring& s)
{
    if (s.empty())
        throw cipher_error("Пустой ключ");

    wstring lower = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    wstring upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    wstring tmp;

    for (auto c : s) {
        size_t pos = lower.find(c);
        if (pos != wstring::npos) {
            tmp.push_back(upper[pos]);
        } else if (alphabet.find(c) != wstring::npos) {
            tmp.push_back(c);
        } else {
            throw cipher_error("Недопустимый ключ");
        }
    }

    int zeroCount = 0;
    for (auto c : tmp) {
        if (c == L'А')
            zeroCount++;
    }
    if (2 * zeroCount > static_cast<int>(tmp.size()))
        throw cipher_error("Вырожденный ключ");

    return tmp;
}

wstring modAlphaCipher::getValidOpenText(const wstring& s)
{
    wstring tmp;
    wstring lower = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    wstring upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

    for (auto c : s) {
        if (alphabet.find(c) != wstring::npos) {
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

wstring modAlphaCipher::getValidCipherText(const wstring& s)
{
    if (s.empty())
        throw cipher_error("Пустой шифротекст");

    for (auto c : s) {
        if (alphabet.find(c) == wstring::npos)
            throw cipher_error("Недопустимый шифротекст");
    }
    return s;
}

wstring modAlphaCipher::encrypt(const wstring& plain)
{
    vector<int> tmp = toNums(getValidOpenText(plain));
    for (unsigned p = 0; p < tmp.size(); ++p) {
        tmp[p] = (tmp[p] + keySeq[p % keySeq.size()]) % alphabet.size();
    }
    return toStr(tmp);
}

wstring modAlphaCipher::decrypt(const wstring& cipher)
{
    vector<int> tmp = toNums(getValidCipherText(cipher));
    for (unsigned p = 0; p < tmp.size(); ++p) {
        tmp[p] = (tmp[p] + alphabet.size() - keySeq[p % keySeq.size()]) % alphabet.size();
    }
    return toStr(tmp);
}
