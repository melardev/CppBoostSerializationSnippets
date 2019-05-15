#pragma once
#include <iostream>
#include <sstream>

#include <boost/date_time.hpp>

#include <boost/smart_ptr/make_shared.hpp>

#include <boost/serialization/shared_ptr.hpp> // we need this if we want to serialize a shared_ptr<AnyClass>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


// I will be using in-memory string serialization to keep it simple, for text based serialization
// using file streams(save file with ofstream and read file with ifstream) look other demo in this same project
// This demo shows how to serialize a shared_ptr<Level>
namespace TextInMemorySerializationInMemoryWithSharedPtr
{
	class Level
	{
	public:
		int level;
		std::string dateAchieved;

		// We always need a default constructor for Boost serialization
		Level() = default;

		Level(int level, const std::string& dateAchieved)
			: level(level), dateAchieved(dateAchieved)
		{
		}

		friend class boost::serialization::access;

		template <class Archive>
		void serialize(Archive& ar, const unsigned int /*version*/)
		{
			ar & level;
			ar & dateAchieved;
		}
	};

	class Character
	{
	public:
		std::string name;
		float health;
		boost::shared_ptr<Level> level;

		Character() = default;

		Character(std::string name, float health, const boost::shared_ptr<Level>& level)
			: name(std::move(name)), health(health),
			  level(level)
		{
		}

	private:
		friend class boost::serialization::access;

		template <class Archive>
		void serialize(Archive& ar, const unsigned int /*version*/)
		{
			ar & name;
			ar & level;
			ar & health;
		}
	};

	std::string save()
	{
		boost::posix_time::ptime currentTime = boost::posix_time::second_clock::local_time();
		std::string currentTimeStr = boost::posix_time::to_simple_string(currentTime);
		boost::shared_ptr<Level> level = boost::make_shared<Level>(55, currentTimeStr);

		Character melardevCharacter(std::string("Melardev"), 75.5f, level);
		std::cout << "Character before serialized" << std::endl;
		std::cout
			<< "Name: " << melardevCharacter.name << std::endl
			<< "Level number: " << melardevCharacter.level->level << std::endl
			<< "Level achieved on : " << melardevCharacter.level->dateAchieved << std::endl
			<< "Health: " << melardevCharacter.health << std::endl;
		std::cout << std::endl << std::endl;

		std::ostringstream outputStringStream;
		{
			// Create a scope
			// This is our serializer
			boost::archive::text_oarchive textOutputArchive(outputStringStream);
			textOutputArchive << melardevCharacter;
		} // End of the scope, the textOutputArchive will be destroyed


		std::cout << "The text based serialization is "
			<< std::endl
			<< outputStringStream.str() << std::endl << std::endl;

		return outputStringStream.str();
	}

	void load(const std::string& serializedStr)
	{
		Character restored;
		std::cout << "Character before state restore" << std::endl;
		std::cout
			<< "Name: " << restored.name << std::endl
			<< "Level pointer: " << restored.level.get() << std::endl
			<< "Health: " << restored.health << std::endl;
		std::cout << std::endl << std::endl;
		{
			// begin scope
			std::istringstream inputStringStream(serializedStr);
			// Our deserializer
			boost::archive::text_iarchive textInputArchive(inputStringStream);
			textInputArchive >> restored;
		} // End scope, archive and stream are closed and destroyed.

		std::cout << "Character after state restore" << std::endl;
		std::cout
			<< "Name: " << restored.name << std::endl
			<< "Level number: " << restored.level->level << std::endl
			<< "Level achieved on : " << restored.level->dateAchieved << std::endl
			<< "Health: " << restored.health << std::endl;
	}

	int main()
	{
		const std::string serializedStr = save();
		load(serializedStr);
		return 0;
	}
}
