/*
 * PugiInterface.hpp
 *
 *  Created on: 31 ott 2019
 *      Author: buzz
 */

#ifndef PUGIINTERFACE_HPP_
#define PUGIINTERFACE_HPP_

#include <boost/format.hpp>
#include "Element.hpp"

class PugiInterface {
	Element root;

	uint8_t current_indent = 0;



private:
	void openNode(Element* element, uint8_t indent) {

	}
	void closeNode();
	void writeAttribute();
	void writeNodeData();

	void processNode(Element node);

public:
	PugiInterface();
	virtual ~PugiInterface();


};



#endif /* PUGIINTERFACE_HPP_ */
