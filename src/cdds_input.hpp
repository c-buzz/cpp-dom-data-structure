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

class cdds_input;


class cdds_input :
	public virtual Element
{
public:
	void process(std::string filename) {


	};
	void process(const std::stringstream& xml_stream) {};


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
	TAttributes xml_attribute_to_element(const pugi::xml_node& xmlNode) {
		// Given and pugi::xml_node, it extracts a TAttributes (map of attributes)

	}

	Element xml_node_to_element(const pugi::xml_node& xmlNode) {
		// Process an XML xml_node and return an Element (recursively for the children)
	}



};



#endif /* SRC_CDDS_INPUT_HPP_ */
