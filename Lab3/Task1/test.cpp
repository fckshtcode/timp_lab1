#include <UnitTest++/UnitTest++.h>
#include <string>
#include <locale>
#include <codecvt>
#include "modAlphaCipher.h"
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
        CHECK_WIDE_EQUAL(L"БВГБВ", modAlphaCipher(L"БВГ").encrypt(L"ААААА"));
    }

    TEST(LongKey) {
        CHECK_WIDE_EQUAL(L"БВГДЕ", modAlphaCipher(L"БВГДЕЁЖЗИЙК").encrypt(L"ААААА"));
    }

    TEST(LowCaseKey) {
        CHECK_WIDE_EQUAL(L"БВГБВ", modAlphaCipher(L"бвг").encrypt(L"ААААА"));
    }

    TEST(DigitsInKey) {
        CHECK_THROW(modAlphaCipher cp(L"Б1"), cipher_error);
    }

    TEST(PunctuationInKey) {
        CHECK_THROW(modAlphaCipher cp(L"Б,В"), cipher_error);
    }

    TEST(WhitespaceInKey) {
        CHECK_THROW(modAlphaCipher cp(L"Б В"), cipher_error);
    }

    TEST(EmptyKey) {
        CHECK_THROW(modAlphaCipher cp(L""), cipher_error);
    }

    TEST(WeakKey) {
        CHECK_THROW(modAlphaCipher cp(L"ААА"), cipher_error);
    }
}

struct KeyB_fixture {
    modAlphaCipher* p;
    KeyB_fixture() { p = new modAlphaCipher(L"Б"); }
    ~KeyB_fixture() { delete p; }
};

SUITE(EncryptTest)
{
    TEST_FIXTURE(KeyB_fixture, UpCaseString) {
        CHECK_WIDE_EQUAL(L"ГТЁНРСЙГЁУ", p->encrypt(L"ВСЕМПРИВЕТ"));
    }

    TEST_FIXTURE(KeyB_fixture, LowCaseString) {
        CHECK_WIDE_EQUAL(L"ГТЁНРСЙГЁУ", p->encrypt(L"всемпривет"));
    }

    TEST_FIXTURE(KeyB_fixture, StringWithWhitespaceAndPunct) {
        CHECK_WIDE_EQUAL(L"РСЙГЁУЕСФД", p->encrypt(L"Привет, друг"));
    }

    TEST_FIXTURE(KeyB_fixture, StringWithNumbers) {
        CHECK_WIDE_EQUAL(L"ТОПГЬНДПЕПН", p->encrypt(L"С Новым 2025 Годом"));
    }

    TEST_FIXTURE(KeyB_fixture, EmptyString) {
        CHECK_THROW(p->encrypt(L""), cipher_error);
    }

    TEST_FIXTURE(KeyB_fixture, NoAlphaString) {
        CHECK_THROW(p->encrypt(L"1234+5678=6912"), cipher_error);
    }

    TEST(MaxShiftKey) {
        CHECK_WIDE_EQUAL(L"БРДЛОПЗБДС", modAlphaCipher(L"Я").encrypt(L"ВСЕМПРИВЕТ"));
    }
}

SUITE(DecryptTest)
{
    TEST_FIXTURE(KeyB_fixture, UpCaseString) {
        CHECK_WIDE_EQUAL(L"ВСЕМПРИВЕТ", p->decrypt(L"ГТЁНРСЙГЁУ"));
    }

    TEST_FIXTURE(KeyB_fixture, LowCaseString) {
        CHECK_THROW(p->decrypt(L"гтёнрсйГЁУ"), cipher_error);
    }

    TEST_FIXTURE(KeyB_fixture, WhitespaceString) {
        CHECK_THROW(p->decrypt(L"ГТЁ НРС ЙГЁУ"), cipher_error);
    }

    TEST_FIXTURE(KeyB_fixture, DigitsString) {
        CHECK_THROW(p->decrypt(L"ТОПГЬНДПЕПН2025"), cipher_error);
    }

    TEST_FIXTURE(KeyB_fixture, PunctString) {
        CHECK_THROW(p->decrypt(L"ГТЁ,НРС"), cipher_error);
    }

    TEST_FIXTURE(KeyB_fixture, EmptyString) {
        CHECK_THROW(p->decrypt(L""), cipher_error);
    }

    TEST(MaxShiftKey) {
        CHECK_WIDE_EQUAL(L"ВСЕМПРИВЕТ", modAlphaCipher(L"Я").decrypt(L"БРДЛОПЗБДС"));
    }
}

int main()
{
    return UnitTest::RunAllTests();
}
