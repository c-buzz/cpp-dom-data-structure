/*
 * TAttribute.hpp
 *
 *  Created on: 25 ott 2019
 *      Author: buzz
 */

#ifndef ATTRIBUTE_HPP_
#define ATTRIBUTE_HPP_

#include <string>
#include <map>


typedef std::map<std::string, std::string> TMapAttributes;

class TAttribute {

private:
	std::string name;
	std::string value;
	bool is_null;

public:
	TAttribute(std::string name = std::string(), std::string value = std::string())
	{
		setName(name);
		setValue(value);
		this->is_null = isNull();
	}

	virtual ~TAttribute() {};

	std::pair<std::string, std::string> get_pair() {
		return std::pair<std::string, std::string>(name, value);
	}

	bool isNull() {
		return name.empty() || value.empty();
	}

	const std::string& getName() const {
		return name;
	}

	void setName(const std::string &name) {
		this->name = name;
	}

	const std::string& getValue() const {
		return value;
	}

	void setValue(const std::string &value) {
		this->value = value;
	}

	void setNull()
	{
		this->value.clear();
		this->name.clear();
	}
};

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

	bool exists(std::string name)
	{
		return !this->getAttribute(name).isNull();
	}

	bool addAttribute(TAttribute attribute)
	{
		// Check that attribute is not null and that there is not another attribute with the same name
		// ToDo: gestione errori
		if (!attribute.isNull() && !this->exists(attribute.getName()))
		{
			map_attributes.insert(attribute.get_pair());
			return true;
		}
		else {
			return false;
		}
	}

	bool addAttribute(std::string name, std::string value) {
		TAttribute attribute(name, value);
		return addAttribute(attribute);
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

	TAttribute getAttribute(std::string name)
	{
		TMapAttributes::iterator iterator = this->map_attributes.find(name);
		TAttribute result = (iterator != map_attributes.end()) ?
				TAttribute(iterator->first, iterator->second) :
				TAttribute();

		return result;
	}


};

#endif /* ATTRIBUTE_HPP_ */
