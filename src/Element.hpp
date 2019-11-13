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
#include <iostream>


#include "boost/any.hpp"


class TAttributes;
class Element;

typedef std::map<std::string, std::string> TMapAttributes;
typedef std::vector<Element> TChildren;
typedef std::vector<uint8_t> cdds_data_t;

class TAttributes
{
private:
	TMapAttributes map_attributes;

public:
	TAttributes() {};
	virtual ~TAttributes() {};

	const TMapAttributes& getMapAttributes() const
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
		if (exists(name) || name.empty()) {
			return false;
		} else {
			map_attributes[name] = value;
			return true;
		}
	}

	void attribute(std::string name, std::string value) {
		map_attributes[name] = value;
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

class Element: public virtual TAttributes {
private:
	std::string name;
	boost::any value;
	TChildren children;

	int element_id;
	Element* parent;

	Element* root;

	static int element_id_counter;

	void _init_ptr() {
		root = nullptr;
		parent = nullptr;
	}

public:
	Element(std::string name, boost::any value) :
			name(name), value(value), element_id(++element_id_counter)
	{
		_init_ptr();
	}

	Element(std::string name) :
		name(name), element_id(++element_id_counter)
	{
		_init_ptr();
	}

	virtual ~Element() {};

	const std::string& getName() const {
		return name;
	}

	void setName(const std::string &name) {
		this->name = name;
	}

	const boost::any& getValue() const {
		return this->value;
	}

	void setValue(boost::any value) {
		this->value = value;
	}

	Element* addChild(Element child) {
		child.parent = this;
		children.push_back(child);
		return &children.back();
	}

	const Element* addChild(std::string name, boost::any value = NULL) {
		Element child(name, value);
		return addChild(child);
	}

	bool hasChild() const {
		return (children.size() > 0);
	}

	bool hasValue() const {
		/*try {
			std::string str = boost::any_cast<std::string>(value);
			std::cout << str << std::endl;
			return !str.empty();
		}
		catch (const boost::bad_any_cast& e) {
			return !value.empty();
		}*/
		return !value.empty();
	}

	int childrenCount() {
		return children.size();
	}

	const TChildren& getChildrenVector() const {
		return this->children;
	}

	Element* findChildByID(int _id, TChildren::iterator& it) {
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

	const std::vector<Element>* childrenVector() {
		const std::vector<Element>* vptr = &children;
		return vptr;
	}
};

int Element::element_id_counter = 0;

#endif /* ELEMENT_HPP_ */
