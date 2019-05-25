// #include "polymorphic_snippet.h"
// This cpp is intended to show how you could split a Polymorphic serialization registration
// between the header and cpp file, in the cpp you would use BOOST_CLASS_EXPORT_IMPLEMENT,
// whereas in the header file you would use BOOST_CLASS_EXPORT_KEY

/*
BOOST_CLASS_EXPORT_IMPLEMENT(SerializationPolymorphism::PacketPlayerMovement);

namespace SerializationPolymorphism
{
	Packet::Packet(const int packet_type) : packet_type_{packet_type}
	{
	}

	int Packet::packet_type() const { return packet_type_; }


	template <typename Archive>
	void Packet::serialize(Archive& ar, const unsigned int version) { ar & packet_type_; }

	
	PacketPlayerMovement::PacketPlayerMovement(double xPos, double yPos) :
		Packet{PACKET_TYPE_MOVEMENT}, y_pos_{yPos}, x_pos_{xPos}
	{
	}

	double PacketPlayerMovement::x_pos() const { return x_pos_; }
	double PacketPlayerMovement::y_pos() const { return y_pos_; }

	template <typename Archive>
	void PacketPlayerMovement::serialize(Archive& ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Packet>(*this);
		ar & x_pos_;
		ar & y_pos_;
	}

};
*/
