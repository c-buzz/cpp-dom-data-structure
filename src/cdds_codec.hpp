/*
 * cdds_codec.hpp
 *
 *  Created on: 10 nov 2019
 *      Author: buzz
 */

#ifndef SRC_CDDS_CODEC_HPP_
#define SRC_CDDS_CODEC_HPP_

#include <boost/any.hpp>
#include <iostream>
#include <string>
#include "Element.hpp"
#include <base64_rfc4648.hpp>

#define CDDS_DATA_TYPE_STDSTRING "std::string"
#define CDDS_DATA_TYPE_VECUINT8_T "std::vector<uint8_t>"
#define CDDS_DATA_TYPE_CHAR_ARRAY "char*"


namespace cdds_codec {

	void print_cast_error(std::string var_type, boost::bad_any_cast e) {
		std::cout << "boost::any value detected as "
				<< var_type
				<< " but boost::any_cast<std::string> failed"
				<< std::endl
				<< e.what()
				<< std::endl;
	}

	std::string no_encoding_possible(const boost::any& value) {
		std::string error = std::string("");

		const std::type_info& ti = value.type();

		error = std::string("At the moment, only the possible type are allowed:\n") +
				"\t" + CDDS_DATA_TYPE_STDSTRING + "\n" +
				"\t" + CDDS_DATA_TYPE_VECUINT8_T + "\n" +
				"\t" + CDDS_DATA_TYPE_CHAR_ARRAY + "\n" +
				"Passed a value of type: " + ti.name() + "\n";
		std::cout << error;
		return error;
	}

	static std::string encodeStringData(Element* data_el, const boost::any& value) {
		try {
			std::string str_value = boost::any_cast<std::string>(value);
			data_el->addAttribute("_cdds_data_type", CDDS_DATA_TYPE_STDSTRING);
			str_value = "<![CDATA[" + str_value +"]]>";
			data_el->setValue(str_value);
			return str_value;
		} catch (boost::bad_any_cast &e) {
			print_cast_error("std::string", e);
			return std::string("_error_");
		}
	}

	/**
	 * std::vector<uint8_t> is encoded with base64
	 */
	static std::string encodeVectorUINT8_T(Element *data_el, const boost::any& value) {
		try {
			cdds_data_t v = boost::any_cast<cdds_data_t>(value);
			data_el->addAttribute("_cdds_data_type", CDDS_DATA_TYPE_VECUINT8_T);
			using base64 = cppcodec::base64_rfc4648;

			std::string str_value = base64::encode(v);
			std::string enc = "<![CDATA[" + str_value +"]]>";
			data_el->setValue(enc);
			return enc;
		} catch (boost::bad_any_cast &e) {
			print_cast_error(CDDS_DATA_TYPE_VECUINT8_T, e);
			return std::string("_error_");
		}
	}

	/**
	 * std::vector<uint8_t> is encoded with base64
	 */
	static std::string encodeArrayOfChar(Element *data_el, const boost::any& value) {
		int data_size = -1;
		// ToDO Implement in class Element the check on _cdds_data_size when receing a char array



		try {
			char* v = boost::any_cast<char*>(value);
			data_el->addAttribute("_cdds_data_type", CDDS_DATA_TYPE_CHAR_ARRAY);
			using base64 = cppcodec::base64_rfc4648;

			// Check if the data_size is specified into data_el. Otherwise data_size will count the first null char
			std::string size_str = data_el->getAttribute("_cdds_data_size");
			if (!size_str.empty()) {
				data_size = std::stoi(size_str);
			} else {
				data_size = 1;
				while (v[data_size-1] != 0) {
					data_size++;
				}
			}

			std::string str_value = base64::encode(v, data_size);
			std::string enc = "<![CDATA[" + str_value +"]]>";
			data_el->setValue(enc);
			return enc;
		} catch (boost::bad_any_cast &e) {
			print_cast_error(CDDS_DATA_TYPE_CHAR_ARRAY, e);
			return std::string("_error_");
		}
	}

	static std::string encodeElementData(Element* data_el, const boost::any& value) {
		const std::type_info& ti = value.type();

		if (ti == typeid(std::string)) {
			return encodeStringData(data_el, value);
		} else if(ti == typeid(char*)) {
			return encodeArrayOfChar(data_el, value);
		} else if (ti == typeid(cdds_data_t)) {
			return encodeVectorUINT8_T(data_el, value);
		} else {
			return no_encoding_possible(value);
		}
	}



};



#endif /* SRC_CDDS_CODEC_HPP_ */
