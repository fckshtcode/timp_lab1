#include <vector>
#include <string>
#include <map>
#include <locale>
#include <codecvt>
#include <stdexcept>
using namespace std;

class cipher_error: public invalid_argument {
public:
    explicit cipher_error (const string& what_arg):
        invalid_argument(what_arg) {}
    explicit cipher_error (const char* what_arg):
        invalid_argument(what_arg) {}
};

class modAlphaCipher
{
    
private:
    wstring alphabet = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    map<wchar_t, int> alphaIndex;
    vector<int> keySeq;
    vector<int> toNums(const wstring& s);
    wstring toStr(const vector<int>& v);
    wstring getValidKey(const wstring& s);
    wstring getValidOpenText(const wstring& s);
    wstring getValidCipherText(const wstring& s);
    
public:
    modAlphaCipher() = delete;
    modAlphaCipher(const wstring& keyStr);
    wstring encrypt(const wstring& plain);
    wstring decrypt(const wstring& cipher);
};
