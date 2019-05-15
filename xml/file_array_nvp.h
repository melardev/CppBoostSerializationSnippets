#pragma once
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>

namespace XmlFileArrayNVPSerialization
{
	void save()
	{
		std::ofstream file("./serialized.xml");
		boost::archive::xml_oarchive oa(file);
		int digits[] = {1, 2, 3, 4, 5, 6};
		oa & BOOST_SERIALIZATION_NVP(digits);
	}

	void load()
	{
		std::ifstream file("./serialized.xml");
		boost::archive::xml_iarchive ia(file);
		// Boost knows how many items there were on the array
		// So if we had less, the remaining elements will be empty,
		// this is why we see in the output random numbers

		// In the other hand, if we create a smaller array, the app will crash
		int restored[10];
		ia >> BOOST_SERIALIZATION_NVP(restored);
		const std::ostream_iterator<int> os_iterator(std::cout, " ");
		std::copy(restored, restored + 10, os_iterator);
	}

	int main()
	{
		save();
		load();
		return 0;
	}
}
