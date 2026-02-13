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

#define CHECK_WIDE_EQUAL(expected, actual) \
    CHECK_EQUAL(wideToUtf8(expected), wideToUtf8(actual))

SUITE(KeyTest)
{
    TEST(ValidKey) {
        CHECK_WIDE_EQUAL(L"ЕРЕСПВВМИТ", Table(3).encrypt(L"ВСЕМПРИВЕТ"));
    }

    TEST(LargeKey) {
        CHECK_WIDE_EQUAL(L"ТЕВИРП", Table(10).encrypt(L"ПРИВЕТ"));
    }

    TEST(KeyEqualsLength) {
        CHECK_WIDE_EQUAL(L"ТЕВИРПМЕСВ", Table(10).encrypt(L"ВСЕМПРИВЕТ"));
    }

    TEST(NegativeKey) {
        CHECK_THROW(Table cp(-3), cipher_error);
    }

    TEST(ZeroKey) {
        CHECK_THROW(Table cp(0), cipher_error);
    }

    TEST(WeakKey) {
        CHECK_THROW(Table cp(1), cipher_error);
    }
}

struct Key3_fixture {
    Table* p;
    Key3_fixture() { p = new Table(3); }
    ~Key3_fixture() { delete p; }
};

SUITE(EncryptTest)
{
    TEST_FIXTURE(Key3_fixture, UpCaseString) {
        CHECK_WIDE_EQUAL(L"ЕРЕСПВВМИТ", p->encrypt(L"ВСЕМПРИВЕТ"));
    }

    TEST_FIXTURE(Key3_fixture, LowCaseString) {
        CHECK_WIDE_EQUAL(L"ЕРЕСПВВМИТ", p->encrypt(L"всемпривет"));
    }

    TEST_FIXTURE(Key3_fixture, StringWithWhitespace) {
        CHECK_WIDE_EQUAL(L"ЕРЕСПВВМИТ", p->encrypt(L"ВСЕМ ПРИВЕТ"));
    }

    TEST_FIXTURE(Key3_fixture, StringWithPunctuation) {
        CHECK_WIDE_EQUAL(L"ИТУРЕРПВДГ", p->encrypt(L"ПРИВЕТ, ДРУГ"));
    }

    TEST_FIXTURE(Key3_fixture, EmptyString) {
        CHECK_THROW(p->encrypt(L""), cipher_error);
    }

    TEST_FIXTURE(Key3_fixture, NoLetters) {
        CHECK_THROW(p->encrypt(L"123"), cipher_error);
    }

    TEST_FIXTURE(Key3_fixture, ShortString) {
        CHECK_WIDE_EQUAL(L"А", p->encrypt(L"А"));
    }

    TEST_FIXTURE(Key3_fixture, TwoCharString) {
        CHECK_WIDE_EQUAL(L"ЫТ", p->encrypt(L"ТЫ"));
    }

    TEST(NonMultipleKey) {
        CHECK_WIDE_EQUAL(L"ПТМЕЕВСИВР", Table(5).encrypt(L"ВСЕМПРИВЕТ"));
    }

    TEST(KeyLargerThanText) {
        CHECK_WIDE_EQUAL(L"ТЕВИРПМЕСВ", Table(11).encrypt(L"ВСЕМПРИВЕТ"));
    }
}

SUITE(DecryptTest)
{
    TEST_FIXTURE(Key3_fixture, UpCaseString) {
        CHECK_WIDE_EQUAL(L"ВСЕМПРИВЕТ", p->decrypt(L"ЕРЕСПВВМИТ"));
    }

    TEST_FIXTURE(Key3_fixture, LowCaseString) {
        CHECK_THROW(p->decrypt(L"ереспввМИТ"), cipher_error);
    }

    TEST_FIXTURE(Key3_fixture, WhitespaceString) {
        CHECK_THROW(p->decrypt(L"ЕРЕ СПВ ВМИТ"), cipher_error);
    }

    TEST_FIXTURE(Key3_fixture, DigitsString) {
        CHECK_THROW(p->decrypt(L"ИТРЕПВ2025"), cipher_error);
    }

    TEST_FIXTURE(Key3_fixture, PunctString) {
        CHECK_THROW(p->decrypt(L"ЕРЕ,СПВ"), cipher_error);
    }

    TEST_FIXTURE(Key3_fixture, EmptyString) {
        CHECK_THROW(p->decrypt(L""), cipher_error);
    }

    TEST_FIXTURE(Key3_fixture, ShortStringDecrypt) {
        CHECK_WIDE_EQUAL(L"А", p->decrypt(L"А"));
    }

    TEST_FIXTURE(Key3_fixture, TwoCharDecrypt) {
        CHECK_WIDE_EQUAL(L"ТЫ", p->decrypt(L"ЫТ"));
    }

    TEST(NonMultipleKeyDecrypt) {
        CHECK_WIDE_EQUAL(L"ВСЕМПРИВЕТ", Table(5).decrypt(L"ПТМЕЕВСИВР"));
    }

    TEST(KeyLargerThanTextDecrypt) {
        CHECK_WIDE_EQUAL(L"ВСЕМПРИВЕТ", Table(11).decrypt(L"ТЕВИРПМЕСВ"));
    }
}

int main()
{
    return UnitTest::RunAllTests();
}
