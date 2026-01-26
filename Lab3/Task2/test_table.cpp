#include <UnitTest++/UnitTest++.h>
#include <string>
#include <locale>
#include <codecvt>
#include "table.h"

using namespace std;

string wideToUtf8(const wstring& ws) {
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(ws);
}

wstring utf8ToWide(const string& s) {
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(s);
}

#define CHECK_WIDE_EQUAL(expected, actual) \
    CHECK_EQUAL(wideToUtf8(expected), wideToUtf8(actual))

SUITE(ConstructorTest)
{
    TEST(ValidKey) {
        Table cipher(3);
        CHECK_WIDE_EQUAL(L"ЕРЕСПВВМИТ", cipher.encrypt(L"ВСЕМПРИВЕТ"));
    }
    
    TEST(LongKey) {
        Table cipher(10);
        CHECK_WIDE_EQUAL(L"ТЕВИРП", cipher.encrypt(L"ПРИВЕТ"));
    }
    
    TEST(KeyEqualsMessageLength) {
        Table cipher(10);
        CHECK_WIDE_EQUAL(L"ТЕВИРПМЕСВ", cipher.encrypt(L"ВСЕМПРИВЕТ"));
    }
    
    TEST(NegativeKey) {
        CHECK_THROW(Table cipher(-3), cipher_error);
    }
    
    TEST(ZeroKey) {
        CHECK_THROW(Table cipher(0), cipher_error);
    }
}

struct Key3Fixture {
    Table* cipher;
    
    Key3Fixture() {
        cipher = new Table(3);
    }
    
    ~Key3Fixture() {
        delete cipher;
    }
};

SUITE(EncryptTest)
{
    TEST_FIXTURE(Key3Fixture, UpperCaseString) {
        CHECK_WIDE_EQUAL(L"ЕРЕСПВВМИТ", cipher->encrypt(L"ВСЕМПРИВЕТ"));
    }
    
    TEST_FIXTURE(Key3Fixture, LowerCaseString) {
        CHECK_WIDE_EQUAL(L"ЕРЕСПВВМИТ", cipher->encrypt(L"всемпривет"));
    }
    
    TEST_FIXTURE(Key3Fixture, StringWithWhitespace) {
        CHECK_WIDE_EQUAL(L"ЕРЕСПВВМИТ", cipher->encrypt(L"ВСЕМ ПРИВЕТ"));
    }
    
    TEST_FIXTURE(Key3Fixture, StringWithNumbers) {
        CHECK_WIDE_EQUAL(L"ИТРЕПВ", cipher->encrypt(L"ПРИВЕТ"));
    }
    
    TEST_FIXTURE(Key3Fixture, EmptyString) {
        CHECK_THROW(cipher->encrypt(L""), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, NoLetters) {
        CHECK_THROW(cipher->encrypt(L"123"), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, StringWithPunctuation) {
        CHECK_WIDE_EQUAL(L"ИТРРЕИПВМ", cipher->encrypt(L"ПРИВЕТ, ДРУГ"));
    }
    
    TEST_FIXTURE(Key3Fixture, ShortString) {
        CHECK_WIDE_EQUAL(L"А", cipher->encrypt(L"А"));
    }
    
    TEST_FIXTURE(Key3Fixture, TwoCharString) {
        CHECK_WIDE_EQUAL(L"ЫТ", cipher->encrypt(L"ТЫ"));
    }

    TEST(NonMultipleKeyLength) {
        Table cipher(5);
        CHECK_WIDE_EQUAL(L"ПТМЕЕВСИВР", cipher.encrypt(L"ВСЕМПРИВЕТ"));
    }
    
    TEST(NonMultipleKeyLength2) {
        Table cipher(11);
        CHECK_WIDE_EQUAL(L"ТЕВИРПМЕСВ", cipher.encrypt(L"ВСЕМПРИВЕТ"));
    }
}

SUITE(DecryptTest)
{
    TEST_FIXTURE(Key3Fixture, UpperCaseString) {
        CHECK_WIDE_EQUAL(L"ВСЕМПРИВЕТ", cipher->decrypt(L"ЕРЕСПВВМИТ"));
    }
    
    TEST_FIXTURE(Key3Fixture, LowerCaseString) {
        CHECK_THROW(cipher->decrypt(L"ереспввМИТ"), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, WhitespaceString) {
        CHECK_THROW(cipher->decrypt(L"ЕРЕ СПВ ВМИТ"), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, DigitsString) {
        CHECK_THROW(cipher->decrypt(L"ИТРЕПВ2025"), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, EmptyString) {
        CHECK_THROW(cipher->decrypt(L""), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, NoLettersDecrypt) {
        CHECK_THROW(cipher->decrypt(L"123"), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, ShortStringDecrypt) {
        CHECK_WIDE_EQUAL(L"А", cipher->decrypt(L"А"));
    }
    
    TEST_FIXTURE(Key3Fixture, TwoCharStringDecrypt) {
        CHECK_WIDE_EQUAL(L"ТЫ", cipher->decrypt(L"ЫТ"));
    }

    TEST_FIXTURE(Key3Fixture, ValidCipherText) {
        CHECK_WIDE_EQUAL(L"ВСЕМ, ПРИВЕТ", cipher->decrypt(L"ЕРЕСПВВМИТ"));
    }

    TEST(NonMultipleKeyLengthDecrypt) {
        Table cipher(5);
        CHECK_WIDE_EQUAL(L"ВСЕМПРИВЕТ", cipher.decrypt(L"ПТМЕЕВСИВР"));
    }
    
    TEST(NonMultipleKeyLengthDecrypt2) {
        Table cipher(11);
        CHECK_WIDE_EQUAL(L"ВСЕМПРИВЕТ", cipher.decrypt(L"ТЕВИРПМЕСВ"));
    }
}

int main(int argc, char** argv)
{
    return UnitTest::RunAllTests();
}
