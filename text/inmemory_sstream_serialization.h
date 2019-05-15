#pragma once
#include <boost/serialization/access.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <sstream>
#include <boost/archive/text_iarchive.hpp>
#include <iostream>

namespace TextSerializationInMemoryBasic
{
	class Character
	{
	public:
		std::string name;
		float health;
		unsigned int level;

		Character()
		{
		};

		Character(std::string name, const unsigned int level, const float health)
			: name(name), health(health), level(level)
		{
		}

	private:
		friend class boost::serialization::access;

		// This method is called by Boost framework when we try to serialize or deserialize.
		// To serialize we have to use the << operator, for deserialization we have to use >> operator
		// The great thing is that boost provides a convenient operator, the &, which uses << or >> as appropiate
		// if we are passed an output archive then & is the same as <<, if it is an input archive then it would be
		// the same as >>
		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & name;
			ar & level;
			ar & health;
		}
	};

	inline std::string save()
	{
		// create class instance
		const Character melardevCharacter("Melardev", 50, 75.5f);

		std::cout << "Character before serialized" << std::endl;
		std::cout
			<< "Name: " << melardevCharacter.name << std::endl
			<< "Level: " << melardevCharacter.level << std::endl
			<< "Health: " << melardevCharacter.health << std::endl;
		std::cout << std::endl << std::endl;

		std::ostringstream outputStringStream;
		// Serialization
		{
			// create and open a character archive for output

			// Create a scope
			// this is our serializer(Output, it writes the class into a file)
			boost::archive::text_oarchive textOutputArchive(outputStringStream);
			// serialize a character
			textOutputArchive << melardevCharacter;
		} // Scope end, any variable in the scope is destroyed, hence streams will be closed

		std::cout << "The text based serialization is "
			<< std::endl
			<< outputStringStream.str() << std::endl << std::endl;

		return outputStringStream.str();
	}

	inline void load(const std::string& serializedStr)
	{
		// Deserialization
		Character restored;
		std::cout << "Character before state restore" << std::endl;
		std::cout
			<< "Name: " << restored.name << std::endl
			<< "Level: " << restored.level << std::endl
			<< "Health: " << restored.health << std::endl;
		std::cout << std::endl << std::endl;

		{
			// create a scope
			// Create a File Input Stream
			std::istringstream inputStringStream(serializedStr);
			// create and open an archive for input
			boost::archive::text_iarchive textInputArchive(inputStringStream);
			// restore state
			textInputArchive >> restored;
		} // end of the scope, variables in that scope will be destroyed, streams closed

		std::cout << "Character after state restore" << std::endl;
		std::cout
			<< "Name: " << restored.name << std::endl
			<< "Level: " << restored.level << std::endl
			<< "Health: " << restored.health << std::endl;
	}

	int main()
	{
		const std::string serializedString = save();
		load(serializedString);
		return 0;
	}
}
