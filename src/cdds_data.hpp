/*
 * cdds_data.hpp
 *
 *  Created on: 23 nov 2019
 *      Author: buzz
 */

#ifndef SRC_CDDS_DATA_HPP_
#define SRC_CDDS_DATA_HPP_

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <sstream>

template<typename T>
class cdds_data {
private:
	T data;

	friend class


public:
	cdds_data(T data);
};

template<typename T>
inline cdds_data<T>::cdds_data(T data):
	data(data)
{

}

#endif /* SRC_CDDS_DATA_HPP_ */
