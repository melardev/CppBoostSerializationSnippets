#pragma once
#include <boost/serialization/access.hpp>
#include <fstream>
#include <iostream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

/**
 * This demo proves that a text serializer can also be used to store binary data, such as a vector<unsigned char> which
 * wil lbe the bytes of an image, but the serialized data that I save in a separate file on purpose, shows clearly that that is
 * not efficient, the serialized file is 500+ bytes for an image of 155 bytes, to serialize this kind of binary data
 * use the binary archives provided by C++ Boost, I have a demo serialize_image_bin_archive_demo.h showing this same
 * example but using binary archives.
 */
namespace ImageFileTextSerialization
{
	class Image
	{
	public:
		Image() = default;
		~Image() = default;
		std::vector<unsigned char> image;
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
			ar & image;
		}
	};

	void save()
	{
		Image image;

		std::ifstream file("D:/temp/calc.png", std::ios::binary | std::ios::ate);
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		// std::vector<unsigned char> buffer(size);
		image.image.resize(size);

		if (!file.read((char*)image.image.data(), size))
		{
			std::cerr << "Could not read image" << std::endl;
		}
		// Serialization
		{
			// create and open a character archive for output
			std::ofstream fileOutputStream("D:/temp/boost_serialize_image.txt");
			// Create a scope
			// this is our serializer(Output, it writes the class into a file)
			boost::archive::text_oarchive textOutputArchive(fileOutputStream);
			// serialize a character
			textOutputArchive << image;
		} // Scope end, any variable in the scope is destroyed, hence streams will be closed
	}

	void load()
	{
		// Deserialization

		Image restored;
		std::cout << sizeof(restored.image) << std::endl;
		std::cout << std::endl << std::endl;
		{
			// create a scope

			// Create a File Input Stream
			std::ifstream fileInputStream("D:/temp/boost_serialize_image.txt");

			// create and open an archive for input
			boost::archive::text_iarchive textInputArchive(fileInputStream);

			// restore state
			textInputArchive >> restored;
		} // end of the 

		std::ofstream outfile("D:/temp/image_restored.png", std::ios::out | std::ios::binary);
		outfile.write((char*)restored.image.data(), restored.image.size());
		outfile.close();
	}

	int main()
	{
		save();
		load();
		return 0;
	}
}
