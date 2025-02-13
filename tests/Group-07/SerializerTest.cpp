#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-07/Team07Library/Serializer.h"

#include <iostream>
#include <random>
#include <fstream>

#include <climits>

std::random_device rd;
std::mt19937_64 gen(rd());

TEST_CASE("Serializer Construction", "[Serializer]")
{
    cse::Serializer Saver(cse::Mode::SAVE);
    cse::Serializer Loader(cse::Mode::LOAD);
}

TEST_CASE("Serializer Save Load Basic", "[Serializer]")
{
    cse::Serializer Saver(cse::Mode::SAVE);
    cse::Serializer Loader(cse::Mode::LOAD);
    const char filename[] = "number.bin";
    int number = 22;
    int result = -1;
    Saver.Serialize(number, filename);
    REQUIRE(result == -1);
    Loader.Serialize(result, filename);
    REQUIRE(result == number);
    std::remove(filename);
}

TEST_CASE("Serializer Save Load Fundamental Types", "[Serializer]")
{
    cse::Serializer Saver(cse::Mode::SAVE);
    cse::Serializer Loader(cse::Mode::LOAD);
    const char filename[] = "result.bin";
    std::remove(filename);
    // Testing Int
    std::uniform_int_distribution<int> ranInt(INT_MIN, INT_MAX);
    int Integer;
    for (int i = 0; i < 100; i++)
    {
        Integer = ranInt(gen);
        int Result;
        Saver.Serialize(Integer, filename);
        Loader.Serialize(Result, filename);
        REQUIRE(Integer == Result);
        std::remove(filename);
    }
    // Testing Char
    std::uniform_int_distribution<char> ranChar('A', 'Z');
    char Character;
    for (int i = 0; i < 100; i++)
    {
        Character = ranChar(gen);
        char Result;
        Saver.Serialize(Character, filename);
        Loader.Serialize(Result, filename);
        REQUIRE(Character == Result);
        std::remove(filename);
    }
    // Testing Long Long
    std::uniform_int_distribution<long long> ranLL(LLONG_MIN, LLONG_MAX);
    long long LL;
    for (int i = 0; i < 100; i++)
    {
        LL = ranLL(gen);
        long long Result;
        Saver.Serialize(LL, filename);
        Loader.Serialize(Result, filename);
        REQUIRE(LL == Result);
        std::remove(filename);
    }
    // Testing Double
    std::uniform_real_distribution<double> ranDBL(-1000.0, 1000.0);
    double Double;
    for (int i = 0; i < 100; i++)
    {
        Double = ranDBL(gen);
        double Result;
        Saver.Serialize(Double, filename);
        Loader.Serialize(Result, filename);
        REQUIRE(Double == Result);
        std::remove(filename);
    }
}

TEST_CASE("Serializer Save Load String", "[Serializer]")
{
    cse::Serializer Saver(cse::Mode::SAVE);
    cse::Serializer Loader(cse::Mode::LOAD);
    const char filename[] = "result.bin";
    std::remove(filename);
    for (int i = 0; i < 100; i++)
    {
        std::uniform_int_distribution<int> ranInt(1, 255);
        std::uniform_int_distribution<char> ranChar(' ', '~');
        int Length = ranInt(gen);
        std::string String = "";
        for (int l = 0; l < Length; l++)
        {
            char Character = ranChar(gen);
            String = String + Character;
        }
        std::string Result;
        Saver.Serialize(String, filename);
        Loader.Serialize(Result, filename);
        REQUIRE(String == Result);
        std::remove(filename);
    }
}