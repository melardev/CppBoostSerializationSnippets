#pragma once
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <sstream>
#include <iostream>

/// Demo showing how you can perform serialization using binary archive and then saving
/// data such as int16_t, string and wstring and then recovering them
/// This demo also shows how you can retrieve the size of serialized data which is
/// very important for sockets, where you have to tell the other end how much data
/// to expect
namespace InMemoryStringsBinarySerialization
{
	std::string save()
	{
		std::ostringstream outputStringStream;

		{
			boost::archive::binary_oarchive binaryOutputStream(outputStringStream);
			binaryOutputStream << static_cast<int16_t>(1992);
			binaryOutputStream << std::string("Hello Ascii");
			binaryOutputStream << std::wstring(L"Hello Unicode");
		}

		std::string serializedStr = outputStringStream.str();
		const char* serialized = serializedStr.data();

		// Seek the pointer to the end, it should be already there anyways
		outputStringStream.seekp(0, std::ios::end);
		// Retrieve the size of the serialized data
		int64_t size = outputStringStream.tellp();

		return outputStringStream.str();
		// return std::pair<int64_t, const char*>(size, outputStringStream.str().c_str());
		// std::pair<int64_t, const char*> bufferPair = { outputStringStream.size(), outputStringStream.c_str() };
	}

	void load(const std::string& serializedStr)
	{
		std::istringstream iss(serializedStr);
		{
			boost::archive::binary_iarchive binaryInputStream(iss);
			int16_t val;
			binaryInputStream >> val;
			std::string ascii;
			std::wstring unicode;
			binaryInputStream >> ascii;
			binaryInputStream >> unicode;

			std::cout << "Deserialized results" << std::endl;
			std::cout << "ascii deserialized: " << ascii << std::endl;
			std::wcout << "unicode deserialized: " << unicode << std::endl;
		}
	}

	int main()
	{
		const std::string serializedStr = save();
		load(serializedStr);
		return 0;
	}
}
