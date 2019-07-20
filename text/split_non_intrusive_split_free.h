#pragma once
#include <boost/serialization/split_free.hpp>
#include <utility>
#include <boost/archive/text_oarchive.hpp>
#include <sstream>
#include <iostream>
#include <boost/archive/text_iarchive.hpp>

namespace SerializationSplitNonIntrusiveNonMacro
{
	class Article
	{
	public:
		Article() = default;

		Article(std::string title) : title(std::move(title))
		{
		}

		std::string title;
	};
}

// Notice this is run outside any custom namespace
namespace boost
{
	namespace serialization
	{
		template <class Archive>
		void serialize(Archive& ar, SerializationSplitNonIntrusiveNonMacro::Article& article,
		               const unsigned int file_version)
		{
			std::cout << "Inside serialize()" << std::endl;
			boost::serialization::split_free(ar, article, file_version);
		}


		template <class Archive>
		void save(Archive& ar, const SerializationSplitNonIntrusiveNonMacro::Article& article, unsigned int version)
		{
			std::cout << "Inside save()" << std::endl;
			ar << article.title;
		}

		template <class Archive>
		void load(Archive& ar, SerializationSplitNonIntrusiveNonMacro::Article& article, unsigned int version)
		{
			std::cout << "Inside load()" << std::endl;
			ar >> article.title;
		}
	} // namespace serialization
} // namespace boost


namespace SerializationSplitNonIntrusiveNonMacro
{
	std::string save()
	{
		Article article("Boost Asio Split non intrusive snippet");
		std::ostringstream outputStringStream;
		{
			// Create a scope
			// This is our serializer
			boost::archive::text_oarchive textOutputArchive(outputStringStream);
			textOutputArchive << article;
		} // End of the scope, the textOutputArchive will be destroyed
		std::cout << "Serialized:" << std::endl;
		std::cout << outputStringStream.str();
		return outputStringStream.str();
	}

	void load(const std::string& serializedStr)
	{
		Article article;
		std::cout << std::endl << std::endl;
		{
			// begin scope
			std::istringstream inputStringStream(serializedStr);
			// Our deserializer
			boost::archive::text_iarchive textInputArchive(inputStringStream);
			textInputArchive >> article;
		} // End scope, archive and the stream is closed and destroyed.

		std::cout << "Article after state restore" << std::endl;
		std::cout << "Title: " << article.title << std::endl;
	}

	void main()
	{
		const std::string serializedStr = save();
		load(serializedStr);
	}
}
