﻿# Introduction
This repository contains Boost Cpp Serialization snippets, I have actually a lot more snippets<br>
but I have to refactor them before publishing so you can follow me on Github to stay tuned.

# Getting Started
For Cpp nowadas there are no excuses to setup your environment in a painless manner, this is why<br>
I recommend using vcpkg, which is what I use, with `vcpkg install boost:x64-windows-static` it downloaded and built<br>
boost libraries. Then I open the vcproject file and add the following inside the Globals section:
```xml
<PropertyGroup Label="Globals"> <-- Find this tag, then in that tag add the two VcpkgTriplet tags-->
............
<VcpkgTriplet Condition="'$(Platform)'=='Win32'">x86-windows-static</VcpkgTriplet>
<VcpkgTriplet Condition="'$(Platform)'=='x64'">x64-windows-static</VcpkgTriplet>
..........
```

This is how I set up my environment, I strongly encourage you to do the same, take a time reading about [vcpkg](https://github.com/microsoft/vcpkg)<br>
and enjoy modern Cpp made easy.

# Notes
- BOOST_SERIALIZATION_SPLIT_FREE should always be executed outside any namespace.
- Don't forget the appropiate includes, if you serialize a shared_ptr<T> you need the serialization/shared_ptr.hpp
header file.
- Never use BOOST_EXPORT_CLASS for a base class.

# Social Media
- [Youtube](https://youtube.com/Melardev)
- [Twitter](https://twitter.com/@melardev)
- [Instagram](https://instagram.com/melar_dev)

# Resources
- [The official docs](https://www.boost.org/doc/libs/1_69_0/libs/serialization/example/)
# TODO
- Port to CMake