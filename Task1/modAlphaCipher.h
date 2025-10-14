#include <vector>
#include <string>
#include <map>
#include <locale>
#include <codecvt>
using namespace std;

class modAlphaCipher
{
private:
    wstring alphabet = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    map<wchar_t, int> alphaIndex;
    vector<int> keySeq;
    vector<int> toNums(const wstring& s);
    wstring toStr(const vector<int>& v);
public:
    modAlphaCipher() = delete;
    modAlphaCipher(const wstring& keyStr);
    wstring encrypt(const wstring& plain);
    wstring decrypt(const wstring& cipher);
};