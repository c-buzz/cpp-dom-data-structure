
#define BOOST_ALL_DYN_LINK
#include "boost/config/user.hpp"
#include <boost/format.hpp>
#include <Element.hpp>
#include <cstdio>
#include <iostream>

using namespace std;

#define BOOST_TEST_MODULE TEST
#include <boost/test/included/unit_test.hpp>

struct AttributeFixture {
	AttributeFixture() {
		attributes = new TAttributes();
	}

	~AttributeFixture() {
		delete attributes;
	}

	TAttributes* attributes;
};

BOOST_AUTO_TEST_SUITE(TS_ATTRIBUTE)


BOOST_AUTO_TEST_CASE(ADD_ATTRIBUTE)
{
	TAttributes a;
	// Add an attribute with name "ATTR_NAME" and value "ATTR_VAL"

	a.addAttribute(std::string("ATTR_NAME"), std::string("ATTR_VAL"));

	BOOST_TEST(a.count() == 1);
	BOOST_TEST(a["ATTR_NAME"] == "ATTR_VAL");
	BOOST_TEST(a["ATTR"] == std::string());

	BOOST_TEST(a.exists("ATTR_NAME"));
	BOOST_TEST(a.addAttribute("", "OKVAL_NONAME") == false);
	BOOST_TEST(a.addAttribute("OKNAME_NOVAL", ""));
	BOOST_TEST(a["OKNAME_NOVAL"].compare("") == 0);
	BOOST_TEST(a.addAttribute("ATTR_NAME", "NOVALID") == false);
	BOOST_TEST(a.removeAttribute("ATTR_NAME"));
	BOOST_TEST(a.exists("ATTR_NAME") == false);

}

BOOST_AUTO_TEST_SUITE_END()
