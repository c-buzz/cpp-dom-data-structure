/*
 * cdds_codec.hpp
 *
 *  Created on: 10 nov 2019
 *      Author: buzz
 */

#ifndef SRC_CDDS_CODEC_OLD_HPP_
#define SRC_CDDS_CODEC_OLD_HPP_

#include <boost/any.hpp>
#include <iostream>
#include <string>
#include <pugixml.hpp>
#include <exception>

#include "Element.hpp"
#include <base64_rfc4648.hpp>

typedef std::string cdds_data_type_t;

struct CDDSException: public std::exception {

	std::string msg;

	CDDSException(std::string msg) {
		this->msg = std::string("CDDSException raised with error:\n\t" + msg);
	}

	const char* what() const throw () {
		return msg.c_str();
	}

};

namespace cdds_data_type {
const std::string std_string = "std::string";
const std::string uint8_t_vector = "std::vector<uint8_t>";
const std::string char_array = "char*";

const std::type_info &ti_std_string(typeid(std::string));
const std::type_info &ti_uint8_t_vector(typeid(std::vector<uint8_t>));
const std::type_info &ti_std_string(typeid(std::string));
}
;

namespace cdds_xml_tag {
const std::string data_node_name = "_cdds_data";
const std::string data_type = "_cdds_data_type";
const std::string data_type_hash = "_cdds_data_type_hash";
}
;

namespace cdds_codec {
void print_cast_error(Element &element, cdds_data_type_t var_type,
		boost::bad_any_cast e) {
	std::cout << "boost::any value of element" << element.getParent()->getName()
			<< " detected as " << var_type
			<< " but boost::any_cast<std::string> failed" << std::endl << '\t'
			<< e.what() << std::endl;
}

std::string no_encoding_possible(const boost::any &value) {
	std::string error = std::string("");

	const std::type_info &ti = value.type();

	error = std::string("At the moment, only the possible types are allowed:\n")
			+ "\t" + cdds_data_type::uint8_t_vector + "\n" + "\t"
			+ cdds_data_type::std_string + "\n" + "\t"
			+ cdds_data_type::char_array + "\n" + "Passed a value of type: "
			+ ti.name() + "\n";
	std::cout << error;
	return error;
}

static std::string encodeStringData(Element *data_el, const boost::any &value) {
	try {
		std::string str_value = boost::any_cast<std::string>(value);
		data_el->addAttribute("_cdds_data_type", cdds_data_type::std_string);
		str_value = "<![CDATA[" + str_value + "]]>";
		data_el->setValue(str_value);
		return str_value;
	} catch (boost::bad_any_cast &e) {
		print_cast_error(*data_el, cdds_data_type::std_string, e);
		return std::string("_error_");
	}
}

/**
 * std::vector<uint8_t> is encoded with base64
 */
static std::string encodeVectorUINT8_T(Element *data_el,
		const boost::any &value) {
	try {
		cdds_data_t v = boost::any_cast<cdds_data_t>(value);

		data_el->addAttribute(cdds_xml_tag::data_type,
				cdds_data_type::uint8_t_vector);
		using base64 = cppcodec::base64_rfc4648;

		std::string str_value = base64::encode(v);
		std::string enc = "<![CDATA[" + str_value + "]]>";
		data_el->setValue(enc);
		return enc;
	} catch (boost::bad_any_cast &e) {
		print_cast_error(*data_el, cdds_data_type::uint8_t_vector, e);
		return std::string("_error_");
	}
}

/**
 * std::vector<uint8_t> is encoded with base64
 */
static std::string encodeArrayOfChar(Element *data_el,
		const boost::any &value) {
	int data_size = -1;
	// ToDO Implement in class Element the check on _cdds_data_size when receing a char array

	try {
		char *v = boost::any_cast<char*>(value);
		data_el->addAttribute(cdds_xml_tag::data_type,
				cdds_data_type::char_array);
		using base64 = cppcodec::base64_rfc4648;

		// Check if the data_size is specified into data_el. Otherwise data_size will count the first null char
		std::string size_str = data_el->getAttribute("_cdds_data_size");
		if (!size_str.empty()) {
			data_size = std::stoi(size_str);
		} else {
			data_size = 1;
			while (v[data_size - 1] != 0) {
				data_size++;
			}
		}

		std::string str_value = base64::encode(v, data_size);
		std::string enc = "<![CDATA[" + str_value + "]]>";
		data_el->setValue(enc);
		return enc;
	} catch (boost::bad_any_cast &e) {
		print_cast_error(*data_el, cdds_data_type::char_array, e);
		return std::string("_error_");
	}
}

static std::string encodeElementData(Element *data_el,
		const boost::any &value) {
	const std::type_info &ti = value.type();



	if (ti == typeid(std::string)) {
		return encodeStringData(data_el, value);
	} else if (ti == typeid(char*) || (ti == typeid(const char*))
			|| (ti == typeid("")) || (ti == typeid(const char[]))
			|| (ti == typeid(char[]))

			) {
		return encodeArrayOfChar(data_el, value);
	} else if (ti == typeid(cdds_data_t)) {
		return encodeVectorUINT8_T(data_el, value);
	} else {
		return no_encoding_possible(value);
	}
}

static void decodeXMLData(pugi::xml_node xml_node, Element &cdds_node) {

}

class CDDSData {
private:
	boost::any data;

	// Check if the passed xml_node is valid
	// TODO: complete
	bool xmlDataNodeValid(const pugi::xml_node &xml_node) {
		if (!std::strcmp(xml_node.name(),
				cdds_xml_tag::data_node_name.c_str())) {
			throw CDDSException(
					"Trying to generate CDDSData from XML node. Data name not match");
			return false;
		}

		return true;
	}

	void decodeFromXML(const pugi::xml_node &xml_node) {
		// TODO To complete
		if (!xmlDataNodeValid(xml_node)) {
			return;
		}

		std::string data_type(
				xml_node.attribute(cdds_xml_tag::data_type.c_str()).value());

	}



	/*bool encode_to_xml() {
		const std::type_info &ti = value.type();

		if (ti == typeid(std::string)) {
			return encodeStringData(data_el, value);
		} else if (ti == typeid(char*) || (ti == typeid(const char*))
				|| (ti == typeid("")) || (ti == typeid(const char[]))
				|| (ti == typeid(char[]))

				) {
			return encodeArrayOfChar(data_el, value);
		} else if (ti == typeid(cdds_data_t)) {
			return encodeVectorUINT8_T(data_el, value);
		} else {
			// Manage error
			//return no_encoding_possible(value);
			return false;
		}
		return true;
	} */

	void encode_std_string(const std::string &casted_string, pugi::xml_node& parent) {

		pugi::xml_node data_node = parent.append_child(cdds_xml_tag::data_node_name.c_str());

		data_node.append_attribute(cdds_xml_tag::data_type.c_str()) = cdds_data_type::std_string.c_str();

		data_node.append_attribute(cdds_xml_tag::data_type_hash.c_str()) = cdds_data_type::ti_std_string.hash_code();

		using base64 = cppcodec::base64_rfc4648;

		std::string encoded_value = base64::encode(casted_string.c_str(), casted_string.size());

		data_node.set_value(encoded_value.c_str());
	}

	void encode_byte_vector(const std::vector<uint8_t> &casted_vector, pugi::xml_node& parent) {
		pugi::xml_node data_node = parent.append_child(cdds_xml_tag::data_node_name.c_str());

		data_node.append_attribute(cdds_xml_tag::data_type.c_str()) = cdds_data_type::uint8_t_vector.c_str();

		data_node.append_attribute(cdds_xml_tag::data_type_hash.c_str()) = cdds_data_type::ti_uint8_t_vector.hash_code();

		using base64 = cppcodec::base64_rfc4648;

		std::string encoded_value = base64::encode(casted_vector);

		data_node.set_value(encoded_value.c_str());
	}

	void raise_cast_error(cdds_data_type_t var_type, boost::bad_any_cast e) {
		const std::type_info& ti = this->data.type();
		std::stringstream ss;
		ss << "boost::any data has type `" << ti.name() << "`"
				<< " but boost::any_cast<" << var_type << " failed" << std::endl << '\t'
				<< e.what() << std::endl;
		throw CDDSException(ss.str());
	}

	bool is_printable() {
		const std::type_info &ti = data.type();

		if (ti == cdds_data_type::ti_uint8_t_vector) {
			return true;
		}

		if (ti == cdds_data_type::std_string) {
			return true;
		}

		return false;
	}

	std::string data_type() {
		const std::type_info &ti = data.type();
		return std::string(ti.name());
	}

public:
	CDDSData(boost::any data) :
			data(data) {

	}

	CDDSData() {
	}

	CDDSData(pugi::xml_node xml_node) {
		decodeFromXML(xml_node);
	}

	const boost::any& getData() const {
		return data;
	}

	void setData(const boost::any &data) {
		this->data = data;
	}
};

}
;

#endif /* SRC_CDDS_CODEC_OLD_HPP_ */
