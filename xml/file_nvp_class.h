#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>


namespace SerializationSaveXmlDemo
{
	class Character
	{
	public:
		std::string name;
		float health;
		unsigned int level;

		// We need a default constructor
		Character()
		{
		};

		Character(std::string name, unsigned int level = 0, float health = 0)
			: name(name), health(health), level(level)
		{
		}

	private:
		friend class boost::serialization::access;
		// This method is called by Boost framework when we try to serialize or deserialize.
		// To serialize we have to use the << operator, for deserialization we have to use >> operator
		// The great thing is that boost provides a convinient operator, the &, which uses << or >> as appropiate
		// if we are passed an output archive then & is the same as <<, if it is an input archive then it would be
		// the same as >>
		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(name);
			ar & BOOST_SERIALIZATION_NVP(level);
			ar & BOOST_SERIALIZATION_NVP(health);
		}
	};

	void save()
	{
		Character melardev("Melardev", 26, 25.0f);
		// make an archive
		std::ofstream outputFileStream("./serialized.xml");
		// assert(outputFileStream.good());
		boost::archive::xml_oarchive outputXmlSerializer(outputFileStream);
		outputXmlSerializer << BOOST_SERIALIZATION_NVP(melardev);

		std::cout << "Original" << std::endl;
		std::cout
			<< "Name: " << melardev.name << std::endl
			<< "Level number: " << melardev.level << std::endl
			<< "Health: " << melardev.health << std::endl;
		std::cout << std::endl << std::endl;
	}

	void load()
	{
		// open the archive
		std::ifstream ifs("./serialized.xml");
		// assert(ifs.good());
		boost::archive::xml_iarchive ia(ifs);
		Character restored;

		ia >> BOOST_SERIALIZATION_NVP(restored);

		std::cout << "Restored" << std::endl;
		std::cout
			<< "Name: " << restored.name << std::endl
			<< "Level number: " << restored.level << std::endl
			<< "Health: " << restored.health << std::endl;
		std::cout << std::endl << std::endl;
	}

	int main()
	{
		save();
		load();
		return 0;
	}
}
