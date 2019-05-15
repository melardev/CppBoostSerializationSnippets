#pragma once
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <iostream>
#include <fstream>

namespace XmlFileStructNVPSerialization
{
	typedef struct Article
	{
		std::string title;
		unsigned int day;
		unsigned int month;
		unsigned int year;

		Article(std::string title, int day, int month, int year) : day(day), month(month), year(year),
		                                                           title(std::move(title))
		{
		}

		Article() : day(1), month(1), year(2000)
		{
		}

		friend std::ostream& operator <<(std::ostream& out, Article& article)
		{
			out <<
				"Title: " << article.title << std::endl <<
				"Day:" << article.day << std::endl <<
				"Month:" << article.month << std::endl <<
				"Year:" << article.year << std::endl << std::endl;
			return out;
		}

		template <class Archive>
		void serialize(Archive& archive, const unsigned int version)
		{
			archive & BOOST_SERIALIZATION_NVP(title);
			archive & BOOST_SERIALIZATION_NVP(day);
			archive & BOOST_SERIALIZATION_NVP(month);
			archive & BOOST_SERIALIZATION_NVP(year);
		}
	} Article;

	void save()
	{
		// Serialize
		std::ofstream fileOutputStream("archive.xml");
		boost::archive::xml_oarchive oa(fileOutputStream);
		Article article("Cpp Boost", 19, 07, 2016);

		std::cout << "Original article" << std::endl << std::endl;
		std::cout << article;
		oa & BOOST_SERIALIZATION_NVP(article);
	}

	void load()
	{
		// Deserialize

		std::ifstream fileInputStream("archive.xml");
		boost::archive::xml_iarchive ia(fileInputStream);
		Article restoredArticle;
		ia >> BOOST_SERIALIZATION_NVP(restoredArticle);

		std::cout << "Restored Article" << std::endl << std::endl;
		std::cout << restoredArticle;
	}

	int main()
	{
		save();
		load();
		return 0;
	}
}
