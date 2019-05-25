#pragma once

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <iostream>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp> // DO NOT FORGET THIS
#include <sstream>

namespace SerializationPolymorphism
{
	const int PACKET_TYPE_MOVEMENT = 1;
	const int PACKET_TYPE_SHOT = 2;
	const int PACKET_TYPE_EXIT_GAME = 3;

	class Packet
	{
	public:
		Packet() = default;

		Packet(const int packet_type) : packet_type_{packet_type}
		{
		}

		virtual ~Packet() = default;
		int packet_type() const { return packet_type_; }


	private:
		friend class boost::serialization::access;

		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version) { ar & packet_type_; }

		int packet_type_;
	};

	class PacketPlayerMovement : public Packet
	{
	public:
		PacketPlayerMovement() = default;

		PacketPlayerMovement(double xPos, double yPos) : Packet{PACKET_TYPE_MOVEMENT}, y_pos_{yPos}, x_pos_{xPos}
		{
		}

		~PacketPlayerMovement() = default;
		double x_pos() const { return x_pos_; }
		double y_pos() const { return y_pos_; }


	private:
		friend class boost::serialization::access;

		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<Packet>(*this);
			ar & x_pos_;
			ar & y_pos_;
		}

		double y_pos_;
		double x_pos_;
	};
}

//BOOST_CLASS_EXPORT(Packet) // NEVER DO THIS

// This is optional in this situation, you can comment it if you want
BOOST_SERIALIZATION_ASSUME_ABSTRACT(SerializationPolymorphism::Packet)

// Register the possible derived classes
BOOST_CLASS_EXPORT(SerializationPolymorphism::PacketPlayerMovement);

// If you prefer you could use the below line in the header,
// and then use BOOST_CLASS_EXPORT_IMPLEMENT(..) in the cpp file
// but since this is a snippets project intended to have self contained demos
// I put everything in the same header file so I use the above approach

// BOOST_CLASS_EXPORT_KEY(SerializationPolymorphism::PacketPlayerMovement)


namespace SerializationPolymorphism
{
	static std::stringstream ss;

	static void save()
	{
		boost::archive::text_oarchive oa{ ss };
		const boost::shared_ptr<Packet> packet(
			new PacketPlayerMovement{ 10.0, 20.0 });
		oa << packet;
		std::cout << "Serialized" << std::endl;
		std::cout << ss.str() << std::endl;
	}

	inline void load()
	{
		boost::archive::text_iarchive ia{ss};
		boost::shared_ptr<Packet> packet;
		ia >> packet;
		const auto packetMovement = boost::dynamic_pointer_cast<PacketPlayerMovement>(packet);
		std::cout << "Packet type" << packet->packet_type() << std::endl;
		std::cout << "X pos " << packetMovement->x_pos() << std::endl;
		std::cout << "Y Pos " << packetMovement->y_pos() << std::endl;
	}

	inline int main()
	{
		save();
		load();
		return 0;
	}
}
