#pragma once
#include <vector>
#include <string>
#include <map>
#include <stdexcept>

class cipher_error: public std::invalid_argument {
public:
    explicit cipher_error (const std::string& what_arg):
        std::invalid_argument(what_arg) {}
    explicit cipher_error (const char* what_arg):
        std::invalid_argument(what_arg) {}
};

class modAlphaCipher
{
private:
    std::wstring alphabet = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    std::map<wchar_t, int> alphaIndex;
    std::vector<int> keySeq;
    std::vector<int> toNums(const std::wstring& s);
    std::wstring toStr(const std::vector<int>& v);
    std::wstring getValidKey(const std::wstring& s);
    std::wstring getValidOpenText(const std::wstring& s);
    std::wstring getValidCipherText(const std::wstring& s);

public:
    modAlphaCipher() = delete;
    modAlphaCipher(const std::wstring& keyStr);
    std::wstring encrypt(const std::wstring& plain);
    std::wstring decrypt(const std::wstring& cipher);
};
