/*
 * PugiInterface.hpp
 *
 *  Created on: 31 ott 2019
 *      Author: buzz
 */

#ifndef PUGIINTERFACE_HPP_
#define PUGIINTERFACE_HPP_

#include <boost/any.hpp>
#include <sstream>
#include <fstream>
#include <base64_rfc4648.hpp>

#include "Element.hpp"
#include "cdds_codec.hpp"

class cdds_output :
		public virtual Element
{
private:
	uint8_t current_indent = 0;
	std::ostringstream os;

	void writeAttributes(const TMapAttributes& attributes) {
		for (auto const &attribute : attributes) {
			std::string name = attribute.first;
			std::string value = attribute.second;
			// write ' attr_name="attr_value"'
			os << ' ' << name << "=\"" << value << '"';
		}
	}

	void writeNodeData(const Element &element) {
		// TODO
		// It functions only if data is a vector of uint8_t
		++current_indent;

		// Check value type_info

		if (element.getName() == "_cdds_data") {
			try {
				std::string data_content = boost::any_cast<std::string>(element.getValue());
				insertTabulation(current_indent);
				os << data_content << "\n";

			} catch (boost::bad_any_cast& e) {
				std::cout << e.what();
			}
		} else {
			Element data_el("_cdds_data");
			cdds_codec::encodeElementData(&data_el, element.getValue());
			processNode(data_el);
		}
		--current_indent;
	}

	inline void insertTabulation(uint8_t ind) {
		if (ind > 0) {
			while (ind > 0) {
				this->os << '\t';
				ind--;
			}
		}
	}

	void processNode(const Element& element) {
		TMapAttributes attributes = element.getMapAttributes();

		// Open the node tag
		insertTabulation(current_indent);
		os << "<" << element.getName();

		// WRITE ATTRIBUTES
		writeAttributes(element.getMapAttributes());

		// Close the tag
		os << ">" << std::endl;

		// WRITE DATA
		if (element.hasValue()) {
			writeNodeData(element);
		}

		// WRITE CHILDREN
		if (element.hasChild()) {
			++current_indent;

			const TChildren& children = element.getChildrenVector();

			for (Element element: children) {
				processNode(element);
			}

			--current_indent;
		}

		insertTabulation(current_indent);

		// Close the node
		os << "</" << element.getName() << ">\n";

	}

	void XMLOverture() {
		// TODO: Manage doctype
		os.clear();
		//os << "<?xml version=\"1.0\" encoding=\"UTF-8\">" << std::endl;
		os << "<!DOCTYPE profiles SYSTEM \"profiles.dtd\">" << std::endl;
	}

	void writeToFile(std::string filename) {
		std::ofstream fstream;
		fstream.open(filename, std::ios::out);
		os << std::ends;
		fstream << os.str() << std::endl;
		fstream.close();
	}

public:
	cdds_output(std::string nodeName, boost::any nodeValue):
		Element(nodeName, nodeValue)
	{

	}

	void processData() {
		XMLOverture();
		processNode(*this);
	}

	void outputXML(std::string filename) {
		if (os.tellp() == 0) {
			processData();
		}
		if (!filename.empty()) {
			writeToFile(filename);
		}
	}

	cdds_data_t get_byte_data() {
		std::string str = os.str();
		cdds_data_t vec(str.begin(), str.end());
		return vec;
	}

	virtual ~cdds_output() {
	}
	;
};

#endif /* PUGIINTERFACE_HPP_ */
