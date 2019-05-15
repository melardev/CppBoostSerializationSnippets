#pragma once
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <iostream>
#include <fstream>

// Refactor the fields to title, description and maybe date
namespace XmlFileStructNVPSerialization
{
	typedef struct Article
	{
		unsigned int m_day;
		unsigned int m_month;
		unsigned int m_year;
		std::string title;

		Article(std::string title, int day, int month, int year) : m_day(day), m_month(month), m_year(year),
		                                                           title(std::move(title))
		{
		}

		Article() : m_day(1), m_month(1), m_year(2000)
		{
		}

		friend std::ostream& operator <<(std::ostream& out, Article& article)
		{
			out <<
				"Title: " << article.title << std::endl <<
				"Day:" << article.m_day << std::endl <<
				"Month:" << article.m_month << std::endl <<
				"Year:" << article.m_year << std::endl << std::endl;
			return out;
		}

		template <class Archive>
		void serialize(Archive& archive, const unsigned int version)
		{
			archive & BOOST_SERIALIZATION_NVP(title);
			archive & BOOST_SERIALIZATION_NVP(m_day);
			archive & BOOST_SERIALIZATION_NVP(m_month);
			archive & BOOST_SERIALIZATION_NVP(m_year);
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
