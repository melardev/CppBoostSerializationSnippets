#pragma once

// Text based serialization
#include "text/inmemory_shared_ptr.h"
#include "text/file_image.h"
#include "text/file_basic.h"
#include "text/inmemory_struct_simple.h"
#include "text/inmemory_sstream_serialization.h"
#include "text/file_basic.h"
#include "text/inmemory_struct_simple.h"
#include "text/inmemory_shared_ptr.h"
#include "text/file_image.h"
#include "text/class_hierarchy_serialization.h"
#include "text/class_hierarchy_serialization.h"
#include "text/inmemory_sstream_serialization.h"
#include "text/polymorphic_shared_ptr/polymorphic_snippet.h"
#include "text/polymorphic_shared_template.h"
#include "text/split_non_intrusive_macro.h"
#include "text/split_non_intrusive_split_free.h"

// XML based serialization
/*
#include "xml/file_nvp_hello.h"
#include "xml/file_nvp_struct.h"
#include "xml/file_nvp_class.h"
#include "xml/file_array_nvp.h"
#include "xml/file_stl_containers_nvp.h"
#include "xml/file_nvp_hello.h"
#include "xml/file_stl_containers_nvp.h"
#include "xml/file_nvp_struct.h"
#include "xml/file_nvp_class.h"
#include "xml/file_array_nvp.h"
*/

// Binary serialization
#include "binary/inmemory_polymorphism_ptr_register_type.h"
#include "binary/image_file_binary_serialization.h"
#include "binary/inmemory_strings_serialization.h"
#include "binary/image_file_binary_serialization.h"
#include "binary/inmemory_strings_serialization.h"


namespace SerializationSnippetsLauncher
{
	static void main()
	{
		// ================================================
		// Serialization demos
		// For the snippets I prefer to use Text archives because they produce a more readable
		// serialization output which you can print into the console or dump into a file.
		// But in real applications I would go with binary archives
		// they produce a smaller sized output

		// Text
		// TextFileSerializationBasic::main();
		// TextSerializationInMemoryBasic::main();
		// TextSerializationOnStructInMemoryBasic::main();
		// TextInMemorySerializationInMemoryWithSharedPtr::main();
		// ImageFileTextSerialization::main();
		// ClassHierarchySerialization::main();
		// SerializationPolymorphism::main();
		// PolymorphicSharedSerialization::main();
		// PolymorphicSharedTemplate::main();
		// SerializationSplitNonIntrusive::main();
		SerializationSplitNonIntrusiveNonMacro::main();

		// PolymorphicSharedTemplate::main();

		// XML I am sorry but for the XML Snippets you have to uncomment the XML includes
		// And comment the other includes, I don't know why but if you use all includes
		// the app does not compile, there should be a problem with the archive includes ...

		// XmlSerializationNVPHelloWorld::main();
		// FileXmlSerializationStlContainers::main();
		// XmlFileStructNVPSerialization::main();
		// SerializationSaveXmlDemo::main();
		// XmlFileArrayNVPSerialization::main();

		// Binary
		// ImageFileBinarySerialization::main();
		// InMemoryStringsBinarySerialization::main();
		// InMemoryPolymorphismPtr::main();
	}
}
