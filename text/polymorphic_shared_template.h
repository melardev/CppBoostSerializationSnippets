#pragma once
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/type_info_implementation.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/make_shared.hpp>

#include <sstream>
#include <iostream>


namespace PolymorphicSharedTemplate
{
	class DataStructure
	{
	public:
		DataStructure(const std::string& structure_name): name(structure_name)
		{
		}

		virtual int size() = 0;

		virtual ~DataStructure()
		{
		}

		std::string name;
	private:
		friend class boost::serialization::access;

		template <class Archive>
		void serialize(Archive& ar, unsigned)
		{
			ar & name;
		}
	};

	BOOST_SERIALIZATION_ASSUME_ABSTRACT(DataStructure)

	/// let's assume this is our Vector class implemented from scratch
	/// a vector must store anything, int, string, double, float, etc.
	/// So we use templates. As said before, I am not gonna implement a Vector here
	/// But I simulate its functionality using std::vector, the important is C++ Boost code
	template <class T>
	class Vector : public DataStructure
	{
	public:
		Vector(): DataStructure("Vector")
		{
		}

		virtual int size() override
		{
			return items.size();
		}

		void push(const T& value)
		{
			items.push_back(value);
		}

		T pop()
		{
			return items.pop_back();
		}

		T const& at(int i) const { return items[i]; }

	private:
		std::vector<T> items;
		friend class boost::serialization::access;

		template <class Archive>
		void serialize(Archive& ar, unsigned)
		{
			// Serialize the abstract class if you want
			ar & boost::serialization::base_object<DataStructure>(*this);

			// boost::serialization::void_cast_register<Vector<T>, DataStructure>();

			ar & items;
		}
	};
}

// We must register any variant type we are gonna use, The one I use in this snippet is <int>
BOOST_CLASS_EXPORT(PolymorphicSharedTemplate::Vector<int>)

// But you could also register <std::string> or <float> or whatever you need
BOOST_CLASS_EXPORT(PolymorphicSharedTemplate::Vector<std::string>)

namespace PolymorphicSharedTemplate
{
	static void main()
	{
		std::stringstream ss;

		{
			boost::archive::text_oarchive output_archive(ss);
			Vector<int>* vec = new Vector<int>();

			std::cout << "Storing 2, 4 and 6 as integers in my Vector class" << std::endl;

			vec->push(2);
			vec->push(4);
			vec->push(6);

			std::cout << "Vector size before serializing " << vec->size() << std::endl;
			boost::shared_ptr<DataStructure> int_vector = boost::shared_ptr<Vector<int>>(vec);
			output_archive << int_vector;
		}

		std::cout << "Serialized: '" << ss.str() << "'\n";

		{
			boost::archive::text_iarchive input_archive(ss);

			boost::shared_ptr<DataStructure> restored;
			input_archive >> restored;

			if (boost::iequals(restored->name, "Vector"))
			{
				std::cout << "Detected a Vector serialized, let's try to un-serialize it" << std::endl;
				boost::shared_ptr<Vector<int>> vector_restored = boost::dynamic_pointer_cast<Vector<int>>(restored);
				if (vector_restored != nullptr)
				{
					std::cout << "Vector deserialized successfully; size: " << vector_restored->size() << std::endl;
					for (int i = 0; i < vector_restored->size(); i++)
					{
						std::cout << "Item[" << i << "] = " << vector_restored->at(i) << std::endl;
					}
				}
			}
			else
			{
				std::cout << "This is not a vector hum... name of the data structure is " << restored->name;
			}
		}
	}
}
