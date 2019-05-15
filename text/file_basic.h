#pragma once
#include <fstream>

// Text based serializer and deserializer, o for output, i for input
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <iostream>

/**
 * This demo shows text serialization in boost, as the docs state, this is an intrusive approach.
 * If you wanna learn the other approaches look the other demos and the official docs obviously
 * The way this demo works if we create an object, we save its state into a file
 * then we deserialize it. I don't really like this code because it uses text based serialization,
 * I rather prefer binary based serialization, but this is easy to read, the other comes in another demo file 
 */
namespace TextFileSerializationBasic
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
		// The great thing is that boost provides a convinient operator, the &, which uses << or >> as appropiate
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


	void save()
	{
		// create class instance
		const Character melardevCharacter("Melardev", 50, 75.5f);


		std::cout << "Character before serialized" << std::endl;
		std::cout
			<< "Name: " << melardevCharacter.name << std::endl
			<< "Level: " << melardevCharacter.level << std::endl
			<< "Health: " << melardevCharacter.health << std::endl;
		std::cout << std::endl << std::endl;

		// Serialization
		{
			// create and open a character archive for output
			std::ofstream fileOutputStream("./boost_serialize_basic_demo.txt");
			// Create a scope
			// this is our serializer(Output, it writes the class into a file)
			boost::archive::text_oarchive textOutputArchive(fileOutputStream);
			// serialize a character
			textOutputArchive << melardevCharacter;
		} // Scope end, any variable in the scope is destroyed, hence streams will be closed
	}

	void load()
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
			std::ifstream fileInputStream("./boost_serialize_basic_demo.txt");
			// create and open an archive for input
			boost::archive::text_iarchive textInputArchive(fileInputStream);
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
		save();
		load();
		return 0;
	}
}
