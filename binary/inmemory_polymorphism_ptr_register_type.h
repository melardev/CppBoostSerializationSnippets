#pragma once
#include <iostream>
#include <vector>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>


namespace InMemoryPolymorphismPtr
{
	class Entity; // Forward declares 
	class MonsterEntity; // so my classes are in your order
	class PlayerEntity;

	// https://stackoverflow.com/questions/17698267/polymorphic-serialization-w-boost
	class Game
	{
		friend class boost::serialization::access;

	public:
		Game() = default;
		~Game() = default;

		std::vector<Entity*> entities_;

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			// Register the types you are gonna use
			ar.template register_type<MonsterEntity>();
			ar.template register_type<PlayerEntity>();
			ar & entities_;
		}
	};

	class Entity
	{
	public:
		Entity() : name("Undefined")
		{
		}

		Entity(const std::string& name) : name(name)
		{
		}

		virtual ~Entity() = default;

		std::string name;
	private:
		friend class boost::serialization::access;

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & name;
		}
	};

	class MonsterEntity : public Entity
	{
	public:
		MonsterEntity() : Entity("Generic Monster"), health(0)
		{
		}

		MonsterEntity(const std::string& name, float health) : Entity(name), health(health)
		{
		}

		~MonsterEntity() = default;
		int health;
	private:
		friend class boost::serialization::access;

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			// Forward serialization to Base class
			ar & boost::serialization::base_object<Entity>(*this);
			// Then serialize our field
			ar & health;
		}
	};

	class PlayerEntity : public Entity
	{
		friend class boost::serialization::access;

	public:
		PlayerEntity() : Entity("Undefined"), level(0) { ; }

		PlayerEntity(const std::string& name, int level) : Entity(name), level(level)
		{
		}

		~PlayerEntity() { ; }
		int level;
	private:

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<Entity>(*this);
			ar & level;
		}
	};

	static std::string save()
	{
		MonsterEntity* monster_entity = new MonsterEntity("XeytanDog", 230.0f);
		PlayerEntity* player_entity = new PlayerEntity("Melardev", 42);

		Game game;
		game.entities_.push_back(monster_entity);
		game.entities_.push_back(player_entity);

		// serialize object into a std::string
		std::string serialized_str;
		boost::iostreams::back_insert_device<std::string> inserter(serialized_str);
		boost::iostreams::stream<boost::iostreams::back_insert_device<std::string>> output_stream(inserter);
		boost::archive::binary_oarchive binary_output_archive(output_stream);

		// Save the Game state
		binary_output_archive & game;
		output_stream.flush();

		// Cleanup Game, let's assume we are done with the game ...
		delete monster_entity;
		delete player_entity;
		return serialized_str;
	}

	static void load(const std::string& serialized_str)
	{
		// Restoring the game state
		boost::iostreams::basic_array_source<char> device(serialized_str.data(), serialized_str.size());
		boost::iostreams::stream<boost::iostreams::basic_array_source<char>> input_stream(device);
		boost::archive::binary_iarchive binary_input_archive(input_stream);

		Game restored_game;
		binary_input_archive & restored_game;

		Entity* monster_entity_restored = restored_game.entities_[0];
		Entity* player_entity_restored = restored_game.entities_[1];

		MonsterEntity* monster_entity_restored_cast = dynamic_cast<MonsterEntity*>(monster_entity_restored);
		PlayerEntity* player_entity_restored_cast = dynamic_cast<PlayerEntity*>(player_entity_restored);

		if (monster_entity_restored_cast != nullptr)
		{
			std::cout << "Monster restored: "
				<< monster_entity_restored_cast->name << std::endl <<
				"Health: " << monster_entity_restored_cast->health << std::endl << std::endl;
		}

		if (player_entity_restored_cast != nullptr)
		{
			std::cout << "Player restored, Name: " << player_entity_restored_cast->name << std::endl
				<< "Level: " << player_entity_restored_cast->level;
		}
	}

	int main()
	{
		const std::string serialized_str = save();

		// ... And then later we want to recover the game state, the only thing we have is
		// a string contained the serialized Game state

		load(serialized_str);

		std::cin.get();
		return 0;
	}
}
