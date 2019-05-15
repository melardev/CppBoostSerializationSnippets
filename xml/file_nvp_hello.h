#pragma once
#include <iostream>
#include <fstream>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

namespace XmlSerializationNVPHelloWorld
{
	inline void save()
	{
		std::ofstream file("./serialized.xml");
		boost::archive::xml_oarchive oa(file);
		std::string contents = "Hello World!\n";
		oa & BOOST_SERIALIZATION_NVP(contents);
	}

	inline void load()
	{
		std::ifstream file("./serialized.xml");
		boost::archive::xml_iarchive ia(file);
		std::string contents;
		ia & BOOST_SERIALIZATION_NVP(contents);
		std::cout << contents << std::endl;
	}

	int main()
	{
		save();
		load();
		return 0;
	}
}
