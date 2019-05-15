#pragma once
#include <boost/serialization/access.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/smart_ptr/make_shared.hpp>

#include <sstream>
#include <boost/archive/text_iarchive.hpp>
#include <iostream>

namespace TextSerializationOnStructInMemoryBasic
{
	struct GeoPosition
	{
		double x;
		double y;
		// Remember we need a default constructor, so make args optional
		GeoPosition(const double x = 0, const double y = 0) : x(x), y(y)
		{
		}

		friend class boost::serialization::access;

		template <class Archive>
		void serialize(Archive& ar, const unsigned int /*version*/)
		{
			ar & x & y;
		}
	};

	struct Person
	{
		std::string firstName;
		std::string lastName;
		boost::shared_ptr<GeoPosition> geoPosition;

	private:
		friend class boost::serialization::access;

		template <class Archive>
		void serialize(Archive& ar, const unsigned int /*version*/)
		{
			ar & firstName;
			ar & lastName;
			ar & geoPosition;
		}
	};

	inline std::string save()
	{
		const Person person{"Melar", "Dev", boost::make_shared<GeoPosition>(30.0, 20.0)};
		std::ostringstream outputStringStream;
		{
			boost::archive::text_oarchive textOutputArchive(outputStringStream);
			textOutputArchive << person;
		}

		std::cout << "Original persona" << std::endl;
		std::cout << "First name: " << person.firstName << std::endl <<
			"Last name: " << person.lastName << std::endl <<
			"GeoPosition: (" << person.geoPosition->x << ", " << person.geoPosition->y << ")"
			<< std::endl;

		std::cout << std::endl << std::endl;
		std::cout << "Serialized Person string" << std::endl;
		std::cout << outputStringStream.str() << std::endl;
		return outputStringStream.str();
	}

	inline void load(const std::string& serializedStr)
	{
		Person restored;
		std::istringstream inputStringStream(serializedStr);
		{
			// Our deserializer
			boost::archive::text_iarchive textInputArchive(inputStringStream);
			textInputArchive >> restored;
		}
		std::cout << std::endl << std::endl;
		std::cout << "Restored person" << std::endl << std::endl;
		std::cout << "First name: " << restored.firstName << std::endl <<
			"Last name: " << restored.lastName << std::endl <<
			"GeoPosition: (" << restored.geoPosition->x << ", " << restored.geoPosition->y << ")"
			<< std::endl;
	}

	int main()
	{
		const std::string serializedStr = save();
		load(serializedStr);
		return 0;
	}
}
