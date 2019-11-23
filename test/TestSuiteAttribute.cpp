
//#define BOOST_ALL_DYN_LINK
#include "boost/config/user.hpp"
#include <boost/format.hpp>
#include "../src/Element.hpp"
#include <cstdio>
#include <iostream>

#include "../src/cdds_output.hpp"
#include "../src/cdds_input.hpp"

using namespace std;

/*#define BOOST_TEST_MODULE TEST
#include <boost/test/included/unit_test.hpp>*/

struct AttributeFixture {
	AttributeFixture() {
		attributes = new TAttributes();
	}

	~AttributeFixture() {
		delete attributes;
	}

	TAttributes* attributes;
};

/*BOOST_AUTO_TEST_SUITE(TS_ATTRIBUTE)


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

BOOST_AUTO_TEST_SUITE_END() */

cdds_data_t strtovec(std::string str) {
	cdds_data_t out(str.begin(), str.end());
	return out;
}


int main() {

	cdds_output root("profiles");

	boost::any root_val = root.getValue();

//	const std::type_info &ti = root_val.type();
//	std::cout << "Root value empty: " << root_val.empty() << std::endl;
//	std::cout << "Root value type: " << ti.name() << std::endl;

	Element t59("profile");

	t59.hasValue();

	t59.addAttribute("name", "RFC6238 SHA1 T=59");

		Element t59sec("secret",strtovec("1234567890"));
		t59sec.addAttribute("type","plaint-text");

	t59sec.hasValue();
	cdds_data_t v = boost::any_cast<cdds_data_t>(t59sec.getValue());


	t59.addChild(t59sec);
	t59.addChild(Element("XO", "30"));

	t59.addChild(Element("digits", strtovec("8")));
	t59.addChild(Element("forcedT", strtovec("59")));
	t59.addChild(Element("hash", strtovec("SHA1")));

	root.addChild(t59);

	//cdds_output cdds(&root, "buzztotp_profiles.xml");

	root.outputXML("buzztotp_profiles.xml");

	//cdds_input ci("filename.xml");

	std::cout << "Fine delle trasmissioni";

	return 0;
}
