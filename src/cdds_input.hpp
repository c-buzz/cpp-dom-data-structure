/*
 * cdds_input.hpp
 *
 *  Created on: 11 nov 2019
 *      Author: buzz
 */

#ifndef SRC_CDDS_INPUT_HPP_
#define SRC_CDDS_INPUT_HPP_

#include <string>
#include <sstream>
#include <fstream>
#include "Element.hpp"

#include <pugixml.hpp>
#include "cdds_codec.old.hpp"

class cdds_input;


class cdds_input :
	public virtual Element
{
public:
	void process(std::string filename) {
		pugi::xml_parse_result result = document.load_file(filename.c_str());

		if (result.status == pugi::xml_parse_status::status_ok) {
			parseDocument();
		}

	};

	// TODO
	void process(const std::stringstream& xml_stream) {};

	void parseDocument() {
		for (pugi::xml_node node = document.first_child(); node; node = node.next_sibling()) {
			xml_node_to_element(node, *this);
		}
	}


	cdds_input(std::string nodeName, boost::any nodeValue)
		: Element(nodeName, nodeValue)
	{

	}

	cdds_input(std::string filename)
		: Element("","")
	{
		process(filename);
	}


private:
	pugi::xml_document document;

	inline void xml_attribute_to_element(const pugi::xml_node& xmlNode, Element& element) {
		// Given and pugi::xml_node, it extracts a TAttributes (map of attributes)
		for (pugi::xml_attribute xml_attr = xmlNode.first_attribute(); xml_attr; xml_attr = xml_attr.next_attribute()) {
			element.addAttribute(std::string(xml_attr.name()), std::string(xml_attr.value()));
		}
	}

	void xml_node_to_element(const pugi::xml_node& xmlNode, Element& parent) {
		// Process an XML xml_node and return an Element (recursively for the children)

		// Check if xmlNode as a data_node child representing the value of the Element node
		if (pugi::xml_node xml_data_node = xmlNode.child(cdds_xml_tag::data_node_name.c_str())) {
			// ... otherwise a new boost::any value will be extracted
			xml_data_node_to_value(xml_data_node, parent);
		}


		// If the node is NOT a data_node (it is a proper node) it will be parsed as a new Element object.
		// Otherwise the node will be ignored. NOTE that in the latter case, the node has been already processed by
		// the previous cycle
		if (std::string(xmlNode.name()).compare(cdds_xml_tag::data_node_name)!= 0) {
			// Process attributes
			xml_attribute_to_element(xmlNode, parent);

			// Set the node name
			parent.setName(std::string(xmlNode.name()));

			// Process children
			for (pugi::xml_node child_node = xmlNode.first_child(); child_node; child_node = child_node.next_sibling()) {
				xml_node_to_element(child_node, parent);
			}
		}

	}

	void xml_data_node_to_value(const pugi::xml_node& xmlNode, Element& cdds_node) {
		boost::any cdds_node_value;

		/** Decoding of node data TODO */



		cdds_node.setValue(cdds_node_value);
	}



};



#endif /* SRC_CDDS_INPUT_HPP_ */
