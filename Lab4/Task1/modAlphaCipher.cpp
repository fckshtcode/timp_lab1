/**
 * @file modAlphaCipher.cpp
 * @brief Реализация класса модифицированного алфавитного шифра
 * @author Ладыгин П.А.
 * @date 17.12.25
 * @version 1.0
 * 
 * Содержит реализацию методов класса modAlphaCipher.
 */

#include "modAlphaCipher.h"
using namespace std;

/**
 * @brief Конструктор класса modAlphaCipher
 * @param keyStr Ключ шифрования в виде строки
 * 
 * Инициализирует карту индексов алфавита и преобразует ключ в числовую последовательность.
 * @throws cipher_error если ключ невалидный
 */
modAlphaCipher::modAlphaCipher(const wstring& keyStr)
{
    // Инициализация карты индексов алфавита
    for (unsigned k = 0; k < alphabet.size(); ++k) {
        alphaIndex[alphabet[k]] = k;
    }
    
    // Преобразование ключа в числовую последовательность
    keySeq = toNums(getValidKey(keyStr));
}

/**
 * @brief Преобразует строку в последовательность числовых индексов
 * @param s Входная строка
 * @return Вектор числовых индексов символов строки
 * 
 * Каждый символ строки заменяется на его индекс в алфавите.
 */
vector<int> modAlphaCipher::toNums(const wstring& s)
{
    vector<int> resultNums;
    resultNums.reserve(s.size());
    
    for (auto sym : s) {
        resultNums.push_back(alphaIndex[sym]);
    }
    
    return resultNums;
}

/**
 * @brief Преобразует последовательность числовых индексов в строку
 * @param v Вектор числовых индексов
 * @return Строка, составленная из символов алфавита по заданным индексам
 */
wstring modAlphaCipher::toStr(const vector<int>& v)
{
    wstring resultStr;
    resultStr.reserve(v.size());
    
    for (auto idx : v) {
        resultStr.push_back(alphabet[idx]);
    }
    
    return resultStr;
}

/**
 * @brief Проверяет и нормализует ключ шифрования
 * @param s Входной ключ
 * @return Валидный ключ в верхнем регистре без пробелов
 * @throws cipher_error в следующих случаях:
 *         - ключ пустой
 *         - ключ содержит пробелы
 *         - ключ содержит недопустимые символы
 *         - ключ состоит из одинаковых символов (слабый ключ)
 */
wstring modAlphaCipher::getValidKey(const wstring& s)
{
    wstring tmp;
    
    // Удаление пробелов из ключа
    for (auto c : s) {
        if (!iswspace(c)) {
            tmp.push_back(c);
        }
    }
    
    // Проверка на пустой ключ
    if (tmp.empty())
        throw cipher_error("Empty key");
    
    // Проверка на наличие пробелов в исходной строке
    for (auto c : s) {
        if (iswspace(c)) {
            throw cipher_error("Whitespace in key");
        }
    }
    
    // Определение строк для преобразования регистра
    wstring lower = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    wstring upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    
    // Преобразование символов ключа
    for (auto & c : tmp) {
        // Преобразование строчных букв в прописные
        size_t pos = lower.find(c);
        if (pos != wstring::npos) {
            c = upper[pos];
        } else if (alphabet.find(c) == wstring::npos) {
            // Проверка на недопустимые символы
            throw cipher_error("Invalid key");
        }
    }
    
    // Проверка на вырожденный ключ (все символы одинаковые)
    bool allSame = true;
    if (tmp.size() > 1) {
        wchar_t firstChar = tmp[0];
        for (size_t i = 1; i < tmp.size(); ++i) {
            if (tmp[i] != firstChar) {
                allSame = false;
                break;
            }
        }
        if (allSame) {
            throw cipher_error("Weak key");
        }
    }
    
    return tmp;
}

/**
 * @brief Проверяет и нормализует открытый текст
 * @param s Входной открытый текст
 * @return Валидный открытый текст в верхнем регистре без пробелов
 * @throws cipher_error если текст пустой после удаления пробелов
 */
wstring modAlphaCipher::getValidOpenText(const wstring& s)
{
    wstring tmp;
    wstring lower = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    wstring upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    
    // Обработка каждого символа текста
    for (auto c : s) {
        if (!iswspace(c)) { // Игнорируем пробелы
            if (alphabet.find(c) != wstring::npos) {
                // Символ уже в верхнем регистре
                tmp.push_back(c);
            } else {
                // Попытка преобразовать строчную букву в прописную
                size_t pos = lower.find(c);
                if (pos != wstring::npos) {
                    tmp.push_back(upper[pos]); 
                }
                // Небуквенные символы игнорируются
            }
        }
    }
    
    // Проверка на пустой текст
    if (tmp.empty())
        throw cipher_error("Empty open text");
        
    return tmp;
}

/**
 * @brief Проверяет и нормализует шифртекст
 * @param s Входной шифртекст
 * @return Валидный шифртекст
 * @throws cipher_error в следующих случаях:
 *         - текст содержит пробелы
 *         - текст содержит недопустимые символы
 *         - текст пустой
 */
wstring modAlphaCipher::getValidCipherText(const wstring& s)
{
    wstring tmp;
    
    // Проверка каждого символа шифртекста
    for (auto c : s) {
        if (iswspace(c)) {
            throw cipher_error("Whitespace in cipher text");
        }
        if (alphabet.find(c) == wstring::npos) {
            throw cipher_error("Invalid character in cipher text");
        }
        tmp.push_back(c);
    }
    
    // Проверка на пустой текст
    if (tmp.empty())
        throw cipher_error("Empty cipher text");
    
    return tmp;
}

/**
 * @brief Шифрует открытый текст
 * @param plain Открытый текст для шифрования
 * @return Зашифрованный текст
 * 
 * Алгоритм шифрования:
 * 1. Текст преобразуется в числовую последовательность
 * 2. К каждому числу прибавляется соответствующее число из ключа
 * 3. Результат берется по модулю размера алфавита
 * 4. Числовая последовательность преобразуется обратно в текст
 */
wstring modAlphaCipher::encrypt(const wstring& plain)
{
    vector<int> tmp = toNums(getValidOpenText(plain));
    
    for (unsigned p = 0; p < tmp.size(); ++p) {
        tmp[p] = (tmp[p] + keySeq[p % keySeq.size()]) % alphabet.size();
    }
    
    return toStr(tmp);
}

/**
 * @brief Расшифровывает шифртекст
 * @param cipher Шифртекст для расшифровки
 * @return Расшифрованный текст
 * 
 * Алгоритм расшифровки:
 * 1. Шифртекст преобразуется в числовую последовательность
 * 2. Из каждого числа вычитается соответствующее число из ключа
 * 3. К результату прибавляется размер алфавита для избежания отрицательных значений
 * 4. Результат берется по модулю размера алфавита
 * 5. Числовая последовательность преобразуется обратно в текст
 */
wstring modAlphaCipher::decrypt(const wstring& cipher)
{
    vector<int> tmp = toNums(getValidCipherText(cipher));
    
    for (unsigned p = 0; p < tmp.size(); ++p) {
        tmp[p] = (tmp[p] + alphabet.size() - keySeq[p % keySeq.size()]) % alphabet.size();
    }
    
    return toStr(tmp);
}
