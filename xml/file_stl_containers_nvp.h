#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

// We have to import the corresponding STL serializers, we use list and vector
// so import both
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>

namespace FileXmlSerializationStlContainers
{
	void save()
	{
		std::ofstream file("./serialized.xml");
		boost::archive::xml_oarchive oa(file);
		double decimal_numbers[] = {15.23, 3.24, 49.99, -45.332, 50.0};
		char chars[] = {'M', 'E', 'L', 'A', 'R', 'D', 'E', 'V', '\0'};
		std::list<double> decimal_numbers_list(decimal_numbers, decimal_numbers + 5);
		std::vector<char> chars_vector(chars, chars + strlen(chars) + 1);
		oa & BOOST_SERIALIZATION_NVP(decimal_numbers_list);
		oa & BOOST_SERIALIZATION_NVP(chars_vector);
	}

	void load()
	{
		std::ifstream file("./serialized.xml");
		boost::archive::xml_iarchive ia(file);
		std::list<double> decimal_numbers_list;
		ia >> BOOST_SERIALIZATION_NVP(decimal_numbers_list);

		std::vector<char> chars_vector;
		ia >> BOOST_SERIALIZATION_NVP(chars_vector);

		const std::ostream_iterator<float> os_iterator(std::cout, " ");
		std::copy(decimal_numbers_list.begin(), decimal_numbers_list.end(), os_iterator);
		std::cout << std::endl;
		std::cout << chars_vector.data() << std::endl;
	}

	int main()
	{
		save();
		load();
		return 0;
	}
}
