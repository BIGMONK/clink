// Copyright (c) 2016 Martin Ridgers
// License: http://opensource.org/licenses/MIT

#include "pch.h"

#include <core/str.h>
#include <core/str_tokeniser.h>

//------------------------------------------------------------------------------
TEST_CASE("str_tokeniser : basic")
{
    str_tokeniser t("a;b;c", ";");

    str<> s;
    REQUIRE(t.next(s)); REQUIRE(s.equals("a") == true);
    REQUIRE(t.next(s)); REQUIRE(s.equals("b") == true);
    REQUIRE(t.next(s)); REQUIRE(s.equals("c") == true);
    REQUIRE(!t.next(s));
}

//------------------------------------------------------------------------------
TEST_CASE("str_tokeniser : multi delims")
{
    str_tokeniser t("a;b-c.d", ";-.");

    str<> s;
    REQUIRE(t.next(s)); REQUIRE(s.equals("a") == true);
    REQUIRE(t.next(s)); REQUIRE(s.equals("b") == true);
    REQUIRE(t.next(s)); REQUIRE(s.equals("c") == true);
    REQUIRE(t.next(s)); REQUIRE(s.equals("d") == true);
    REQUIRE(!t.next(s));
}

//------------------------------------------------------------------------------
TEST_CASE("str_tokeniser : ends")
{
    auto inputs = { "a;b;c", ";a;b;c", "a;b;c;" };
    for (auto input : inputs)
    {
        str_tokeniser t(input, ";");

        str<> s;
        REQUIRE(t.next(s)); REQUIRE(s.equals("a") == true);
        REQUIRE(t.next(s)); REQUIRE(s.equals("b") == true);
        REQUIRE(t.next(s)); REQUIRE(s.equals("c") == true);
        REQUIRE(!t.next(s));
    }
}

//------------------------------------------------------------------------------
TEST_CASE("str_tokeniser : delim runs")
{
    auto inputs = { "a;;b--c", "-;a;-b;c", "a;b;-c-;" };
    for (auto input : inputs)
    {
        str_tokeniser t(input, ";-");

        str<> s;
        REQUIRE(t.next(s)); REQUIRE(s.equals("a") == true);
        REQUIRE(t.next(s)); REQUIRE(s.equals("b") == true);
        REQUIRE(t.next(s)); REQUIRE(s.equals("c") == true);
        REQUIRE(!t.next(s));
    }
}

//------------------------------------------------------------------------------
TEST_CASE("str_tokeniser : quote")
{
    str_tokeniser t("'-abc';(-abc);'-a)b;c", ";-");
    t.add_quote_pair("'");
    t.add_quote_pair("()");

    str<> s;
    REQUIRE(t.next(s)); REQUIRE(s.equals("'-abc'") == true);
    REQUIRE(t.next(s)); REQUIRE(s.equals("(-abc)") == true);
    REQUIRE(t.next(s)); REQUIRE(s.equals("'-a)b;c") == true);
    REQUIRE(!t.next(s));
}

//------------------------------------------------------------------------------
TEST_CASE("str_tokeniser : delim return")
{
    str_tokeniser t("a;b;-c-;d", ";-");

    str<> s;
    REQUIRE(t.next(s).delim == ';'); REQUIRE(s.equals("a") == true);
    REQUIRE(t.next(s).delim == ';'); REQUIRE(s.equals("b") == true);
    REQUIRE(t.next(s).delim == '-'); REQUIRE(s.equals("c") == true);
    REQUIRE(t.next(s).delim == '-'); REQUIRE(s.equals("d") == true);
    REQUIRE(!t.next(s));
}
