/*
 * Element.hpp
 *
 *  Created on: 24 ott 2019
 *      Author: buzz
 */

#ifndef ELEMENT_HPP_
#define ELEMENT_HPP_

#include <string>
#include <map>
#include <vector>
#include <functional>


#include "boost/any.hpp"

typedef std::map<std::string, std::string> TMapAttributes;

class TAttributes
{
private:
	TMapAttributes map_attributes;

public:

	TAttributes() {};
	virtual ~TAttributes() {};

	TMapAttributes getMapAttributes()
	{
		return map_attributes;
	}

	std::string operator[] (std::string name) {
		TMapAttributes::iterator it;
		return (exists(name, it)) ? it->second : std::string();
	}

	bool exists(std::string attr_name, TMapAttributes::iterator& iterator)
	{
		iterator = map_attributes.find(attr_name);
		return (iterator != map_attributes.end());
	}

	bool exists(std::string attr_name) {
		TMapAttributes::iterator it;
		return exists(attr_name, it);
	}

	TMapAttributes::size_type count() {
		return map_attributes.size();
	}

	bool addAttribute(std::string name, std::string value) {
		if (name.empty() || exists(name)) {
			return false;
		} else {
			map_attributes.insert(std::pair<std::string,std::string>(name, value));
			return true;
		}
	}

	bool removeAttribute(std::string name)
	{
		TMapAttributes::iterator iterator = this->map_attributes.find(name);
		if (iterator != this->map_attributes.end()) {
			this->map_attributes.erase(iterator);
			return true;
		}
		else {
			return false;
		}
	}

	std::string getAttribute(std::string name)
	{
		TMapAttributes::iterator iterator = this->map_attributes.find(name);
		return (iterator != map_attributes.end()) ?
				iterator->second :
				std::string();
	}


};

class Element: public TAttributes {
private:
	std::string name;
	boost::any value;
	std::vector<Element> children;

	int element_id;
	Element* parent;

	Element* root;

	static int element_id_counter;

public:
	Element(std::string name, boost::any value) :
			name(name), value(value), element_id(++element_id_counter)
	{
		//TODO: gestire name null
		parent = nullptr;
		root = nullptr;
	}

	virtual ~Element() {};

	const std::string& getName() const {
		return name;
	}

	void setName(const std::string &name) {
		this->name = name;
	}

	const boost::any& getValue() {
		return this->value;
	}

	void setValue(boost::any value) {
		this->value = value;
	}

	void addChild(Element child) {
		child.parent = this;
		children.push_back(child);
	}

	bool hasChild() {
		return (children.size() > 0);
	}

	int childrenCount() {
		return children.size();
	}

	Element* findChildByID(int _id, std::vector<Element>::iterator& it) {
		//std::vector<Element>::iterator it;
		it = std::find_if(children.begin(), children.end(),
				[_id](const Element e) {
					return (e.getId() == _id);
				}
		);
		return (it != children.end()) ? &(*it) : nullptr;
	}

	Element* findChildByID(int _id) {
		std::vector<Element>::iterator it;
		return findChildByID(_id, it);
	}

	std::vector<Element*> findChildren(std::string name) {
		std::vector<Element*> result;

		std::vector<Element>::iterator it, begin = children.begin();

		auto lambda_search_by_name = [name](const Element e) {
			return (e.getName().compare(name) == 0);
		};

		while ((it = std::find_if(begin, children.end(), lambda_search_by_name))
				!= children.end()) {
			result.push_back(&(*it));
			begin = it;
		}

		return result;
	}

	std::vector<Element*> find(std::string name) {
		std::vector<Element*> result;

		for (Element child : children) {
			std::vector<Element*> temp_result;
			if (!child.hasChild()) {
				temp_result = child.findChildren(name);
			} else {
				temp_result = child.find(name);
			}
			result.insert(result.end(), temp_result.begin(), temp_result.end());
		}
		return result;
	}

	/**
	 * Delete all children named _name_
	 * 		findChildren(name) retrieves the children of the element
	 * 		foreach element deleteChild by iD
	 * 	return true if any deleting-children found
	 * 	return false if no deleting-children found
	 */
	bool deleteChildren(std::string name)
	{
		std::vector<Element*> children = findChildren(name);
		for (Element* child : children) {
			deleteChild(child->getId());
		}
		return children.size() > 0;
	}

	bool deleteChild(int id)
	{
		std::vector<Element>::iterator it;
		Element* element = findChildByID(id, it);
		if (element != nullptr)	{
			children.erase(it);
			return true;
		} else {
			return false;
		}
	}

	int getId() const {
		return element_id;
	}

	Element* getParent() {
		return parent;
	}

	Element* getRoot() {
		return (parent == nullptr) ? this : parent->getRoot();
	}

	int getRootId() {
		return getRoot()->getId();
	}
};

int Element::element_id_counter = 0;

#endif /* ELEMENT_HPP_ */
