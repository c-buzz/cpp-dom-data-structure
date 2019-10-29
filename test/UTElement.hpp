/*
 * UTElement.cpp
 *
 *  Created on: 26 ott 2019
 *      Author: buzz
 */

#ifndef UTELEMENT_HPP_
#define UTELEMENT_HPP_

/*
 * Action to be tested:
 * 		E1) Create element by name
 * 		E2) Create element without name
 * 		E3) Add child node
 * 		E4) hasChild() and childrenCount()
 * 		E5) hasAttributes()
 * 		E6) find child node by ID
 * 			E6.1) existing child
 * 			E6.2) non-existing child
 * 		E7) find children by name
 * 			E7.1) with no matches
 * 			E7.1) with matches
 * 		E8) find
 * 			E8.1) single level (result must be equal to findChildren)
 * 			E8.2) multiple level
 * 			E8.3) no children
 * 		...
 * 		...
 *
 * 		-) ID Verification
 *
 */

#include <Element.hpp>
#include <cstdio>
#include <iostream>

using namespace std;

void UTElement() {
	// E1
	Element element("TEST_NAME", "TEST_VALUE");
	std::cout << "Creato elemento con NOME = TEST_NAME e Valore = TEST_VALUE" << std::endl;

	// E2
	/*
	Element elementNoName();
	 * TODO
	 *
	 * it call the destructor ~Element(); instead of returning error
	 */

	element.addChild(Element("LIV1_N1", "LIV1_N1"));

	//E5
	if (element.hasChild() == false) {
		std::cout << "OK - hasAttributes() return false" << std::endl;
	}

	TAttribute attr;

	element.addAttribute(TAttribute("ATTR1", "VALATTR1"));
	cout << "- Added attribute (ATTR1,VALATTR1)" << endl;

	attr = element.getAttribute("ATTR1");
	cout << "- getAttribute('ATTR1') returns value " << attr.getValue() << endl;

	element.addAttribute(TAttribute("ATTR2", "VALATTR2"));
	cout << "- Added attribute (ATTR2,VALATTR2)" << endl;

	attr = element.getAttribute("ATTR2");
	cout << "- getAttribute('ATTR2') returns value " << attr.getValue() << endl;

	element.addAttribute(TAttribute("ATTR3", "VALATTR3"));
	cout << "- Added attribute (ATTR3,VALATTR3)" << endl;

	attr = element.getAttribute("ATTR3");
	cout << "- getAttribute('ATTR3') returns value " << attr.getValue() << endl;

}

void testRootId()
{
	Element rootElement("root", "root");
	cout << "Root element ID = " << rootElement.getId() << endl;

	Element* ep = &rootElement;

	// 10 livelli di children
	for (int i = 0; i < 10; i++) {
		Element e("Livello", i);
		cout << "ELEMENTO DI LIVELLO " << i + 2 << endl;
		cout << "ID -> " << e.getId() << endl;
		ep->addChild(e);
		ep = ep->findChildByID(e.getId());
		cout << "ID ROOT -> " << ep->getRootId() << endl << endl;
	}
}

#endif /* UTELEMENT_HPP_ */



