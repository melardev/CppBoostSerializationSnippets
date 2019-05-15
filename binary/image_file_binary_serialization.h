#pragma once
#pragma once
#include <boost/serialization/access.hpp>
#include <fstream>
#include <iostream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>

namespace ImageFileBinarySerialization
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
		// The great thing is that boost provides a convenient operator, the &, which uses << or >> as appropiate
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

		image.image.resize(size);

		if (!file.read((char*)image.image.data(), size))
		{
			std::cerr << "Could not read image" << std::endl;
		}
		// Serialization
		{
			// ios::ate is not required, but ios::binary is, otherwise the image will be corrupted
			// create and open a character archive for output
			std::ofstream fileOutputStream("D:/temp/boost_serialize_image.txt", std::ios::binary | std::ios::ate);
			// Create a scope
			// this is our serializer(Output, it writes the class into a file)
			boost::archive::binary_oarchive textOutputArchive(fileOutputStream);
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
			// ios::binary is required, otherwise the image will be corrupted
			// Create a File Input Stream
			std::ifstream fileInputStream("D:/temp/boost_serialize_image.txt", std::ios::binary);

			// create and open an archive for input
			boost::archive::binary_iarchive textInputArchive(fileInputStream);
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
