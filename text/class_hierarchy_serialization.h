#pragma once
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <iostream>
#include <sstream>


namespace ClassHierarchySerialization
{
	const int PACKET_TYPE_MOVEMENT = 1;
	const int PACKET_TYPE_SHOT = 2;
	const int PACKET_TYPE_EXIT_GAME = 3;

	std::stringstream ss;

	class Packet
	{
	public:
		Packet() = default;

		Packet(const int packet_type) : packet_type_{packet_type}
		{
		}

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

		PacketPlayerMovement(double xPos, double yPos) :
			Packet{PACKET_TYPE_MOVEMENT}, y_pos_{yPos}, x_pos_{xPos}
		{
		}

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

	void save()
	{
		boost::archive::text_oarchive oa{ss};
		const PacketPlayerMovement packet{10.0, 20.0};
		oa << packet;
	}

	void load()
	{
		boost::archive::text_iarchive ia{ss};
		PacketPlayerMovement packet;
		ia >> packet;
		std::cout << "Packet type" << packet.packet_type() << std::endl;
		std::cout << "X pos " << packet.x_pos() << std::endl;
		std::cout << "Y Pos " << packet.y_pos() << std::endl;

	}

	int main()
	{
		save();
		load();
		return 0;
	}
}
