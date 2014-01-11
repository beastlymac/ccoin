#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include "util.h"

BOOST_AUTO_TEST_SUITE(getarg_tests)

static void
ResetArgs(const std::string& strArg)
{
    std::vector<std::string> vecArg;
    boost::split(vecArg, strArg, boost::is_space(), boost::token_compress_on);

    // Insert dummy executable name:
    vecArg.insert(vecArg.begin(), "testbitcoin");

    // Convert to char*:
    std::vector<const char*> vecChar;
    BOOST_FOREACH(std::string& s, vecArg)
        vecChar.push_back(s.c_str());

    ParseParameters(vecChar.size(), &vecChar[0]);
}

BOOST_AUTO_TEST_CASE(boolarg)
{
    ResetArgs("-CCO");
    BOOST_CHECK(GetBoolArg("-CCO"));
    BOOST_CHECK(GetBoolArg("-CCO", false));
    BOOST_CHECK(GetBoolArg("-CCO", true));

    BOOST_CHECK(!GetBoolArg("-fo"));
    BOOST_CHECK(!GetBoolArg("-fo", false));
    BOOST_CHECK(GetBoolArg("-fo", true));

    BOOST_CHECK(!GetBoolArg("-CCOo"));
    BOOST_CHECK(!GetBoolArg("-CCOo", false));
    BOOST_CHECK(GetBoolArg("-CCOo", true));

    ResetArgs("-CCO=0");
    BOOST_CHECK(!GetBoolArg("-CCO"));
    BOOST_CHECK(!GetBoolArg("-CCO", false));
    BOOST_CHECK(!GetBoolArg("-CCO", true));

    ResetArgs("-CCO=1");
    BOOST_CHECK(GetBoolArg("-CCO"));
    BOOST_CHECK(GetBoolArg("-CCO", false));
    BOOST_CHECK(GetBoolArg("-CCO", true));

    // New 0.6 feature: auto-map -nosomething to !-something:
    ResetArgs("-noCCO");
    BOOST_CHECK(!GetBoolArg("-CCO"));
    BOOST_CHECK(!GetBoolArg("-CCO", false));
    BOOST_CHECK(!GetBoolArg("-CCO", true));

    ResetArgs("-noCCO=1");
    BOOST_CHECK(!GetBoolArg("-CCO"));
    BOOST_CHECK(!GetBoolArg("-CCO", false));
    BOOST_CHECK(!GetBoolArg("-CCO", true));

    ResetArgs("-CCO -noCCO");  // -CCO should win
    BOOST_CHECK(GetBoolArg("-CCO"));
    BOOST_CHECK(GetBoolArg("-CCO", false));
    BOOST_CHECK(GetBoolArg("-CCO", true));

    ResetArgs("-CCO=1 -noCCO=1");  // -CCO should win
    BOOST_CHECK(GetBoolArg("-CCO"));
    BOOST_CHECK(GetBoolArg("-CCO", false));
    BOOST_CHECK(GetBoolArg("-CCO", true));

    ResetArgs("-CCO=0 -noCCO=0");  // -CCO should win
    BOOST_CHECK(!GetBoolArg("-CCO"));
    BOOST_CHECK(!GetBoolArg("-CCO", false));
    BOOST_CHECK(!GetBoolArg("-CCO", true));

    // New 0.6 feature: treat -- same as -:
    ResetArgs("--CCO=1");
    BOOST_CHECK(GetBoolArg("-CCO"));
    BOOST_CHECK(GetBoolArg("-CCO", false));
    BOOST_CHECK(GetBoolArg("-CCO", true));

    ResetArgs("--noCCO=1");
    BOOST_CHECK(!GetBoolArg("-CCO"));
    BOOST_CHECK(!GetBoolArg("-CCO", false));
    BOOST_CHECK(!GetBoolArg("-CCO", true));

}

BOOST_AUTO_TEST_CASE(stringarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-CCO", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-CCO", "eleven"), "eleven");

    ResetArgs("-CCO -bar");
    BOOST_CHECK_EQUAL(GetArg("-CCO", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-CCO", "eleven"), "");

    ResetArgs("-CCO=");
    BOOST_CHECK_EQUAL(GetArg("-CCO", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-CCO", "eleven"), "");

    ResetArgs("-CCO=11");
    BOOST_CHECK_EQUAL(GetArg("-CCO", ""), "11");
    BOOST_CHECK_EQUAL(GetArg("-CCO", "eleven"), "11");

    ResetArgs("-CCO=eleven");
    BOOST_CHECK_EQUAL(GetArg("-CCO", ""), "eleven");
    BOOST_CHECK_EQUAL(GetArg("-CCO", "eleven"), "eleven");

}

BOOST_AUTO_TEST_CASE(intarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-CCO", 11), 11);
    BOOST_CHECK_EQUAL(GetArg("-CCO", 0), 0);

    ResetArgs("-CCO -bar");
    BOOST_CHECK_EQUAL(GetArg("-CCO", 11), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);

    ResetArgs("-CCO=11 -bar=12");
    BOOST_CHECK_EQUAL(GetArg("-CCO", 0), 11);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 12);

    ResetArgs("-CCO=NaN -bar=NotANumber");
    BOOST_CHECK_EQUAL(GetArg("-CCO", 1), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);
}

BOOST_AUTO_TEST_CASE(doubledash)
{
    ResetArgs("--CCO");
    BOOST_CHECK_EQUAL(GetBoolArg("-CCO"), true);

    ResetArgs("--CCO=verbose --bar=1");
    BOOST_CHECK_EQUAL(GetArg("-CCO", ""), "verbose");
    BOOST_CHECK_EQUAL(GetArg("-bar", 0), 1);
}

BOOST_AUTO_TEST_CASE(boolargno)
{
    ResetArgs("-noCCO");
    BOOST_CHECK(!GetBoolArg("-CCO"));
    BOOST_CHECK(!GetBoolArg("-CCO", true));
    BOOST_CHECK(!GetBoolArg("-CCO", false));

    ResetArgs("-noCCO=1");
    BOOST_CHECK(!GetBoolArg("-CCO"));
    BOOST_CHECK(!GetBoolArg("-CCO", true));
    BOOST_CHECK(!GetBoolArg("-CCO", false));

    ResetArgs("-noCCO=0");
    BOOST_CHECK(GetBoolArg("-CCO"));
    BOOST_CHECK(GetBoolArg("-CCO", true));
    BOOST_CHECK(GetBoolArg("-CCO", false));

    ResetArgs("-CCO --noCCO");
    BOOST_CHECK(GetBoolArg("-CCO"));

    ResetArgs("-noCCO -CCO"); // CCO always wins:
    BOOST_CHECK(GetBoolArg("-CCO"));
}

BOOST_AUTO_TEST_SUITE_END()
