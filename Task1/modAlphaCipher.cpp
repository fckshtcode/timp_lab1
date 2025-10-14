#include "modAlphaCipher.h"
using namespace std;

modAlphaCipher::modAlphaCipher(const wstring& keyStr)
{
    for (unsigned k = 0; k < alphabet.size(); ++k) {
        alphaIndex[alphabet[k]] = k;
    }
    keySeq = toNums(keyStr);
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

wstring modAlphaCipher::encrypt(const wstring& plain)
{
    vector<int> tmp = toNums(plain);
    for (unsigned p = 0; p < tmp.size(); ++p) {
        tmp[p] = (tmp[p] + keySeq[p % keySeq.size()]) % alphabet.size();
    }
    return toStr(tmp);
}

wstring modAlphaCipher::decrypt(const wstring& cipher)
{
    vector<int> tmp = toNums(cipher);
    for (unsigned p = 0; p < tmp.size(); ++p) {
        tmp[p] = (tmp[p] + alphabet.size() - keySeq[p % keySeq.size()]) % alphabet.size();
    }
    return toStr(tmp);
}