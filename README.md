MemoryCommandoCPP
=================

Process’ virtual memory editing library for Windows in C++20 using WinAPI.

Introduction
------------

I created this project because I wanted an easy to use library, which would work
on x86 and x64 processes, support memory scanning, and for which I wouldn’t have
to manually calculate module’s base address. Couldn’t find one on the Internet,
so I wrote my own.

Features
--------

Works on internal and external virtual process memory.  
Works on x86 and x64 system architectures and for x86 and x64 processes.  
Supports scanning for arrays of bytes and structures.  
Quality of life method overloads which read, edit or scan memory based on just
an address, base address and offset, base address and vector of offsets, module
name and offset, module name and vector of offsets.

Dependencies
------------

[Windows Implementation Libraries (WIL)](https://github.com/microsoft/wil). You
can get this one from NuGet.

[Boost](https://www.boost.org/). You can find Boost in NuGet as well but NuGet’s
version won’t work in this project. NuGet’s version is a header-only version.
This project requires compiled version with headers that are missing in NuGet’s
version. The simplest way to get Boost is to install it using vcpkg package
manager. If you intend to use this library for x86 processes, you need to
install x86-version of Boost: vcpkg install boost . For x64, you need to install
x64 version respectively: vcpkg install boost:x64-windows.

Using the library
-----------------

To start using the library, you need to add MemoryCommando project do your
solution, add reference to it in your project and install [Windows
Implementation Libraries] NuGet package in the MemoryCommando and your project.
I use this library as a git submodule in my projects.

Examples of use
---------------

### Internal

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include "../MemoryCommandoCPP/MemoryCommando/MemoryCommando.h"

int main() {
    const MemoryCommando::MemoryCommando memoryCommando; // define memoryCommando variable

    // define integer variable and its address
    int originalInteger = 5;
    const uintptr_t addressOfOriginalInteger = uintptr_t(&originalInteger);

    // Show original integer and its address
    std::cout << "Original integer:" << std::endl;
    std::cout << "Address: " << std::hex << addressOfOriginalInteger << std::dec << ", Value: " << originalInteger << std::endl;
    std::cout << std::endl;

    // Read original integer's value from memory into a new variable and show its value
    int readInteger = memoryCommando.ReadVirtualMemory<int>(addressOfOriginalInteger);
    std::cout << "readInteger: " << readInteger << std::endl;
    std::cout << std::endl;

    // Write new value to originalInteger variable
    memoryCommando.WriteVirtualMemory<int>(addressOfOriginalInteger, static_cast<int>(8));

    // Show original integer and its address
    std::cout << "Overwritten integer:" << std::endl;
    std::cout << "Address: " << std::hex << addressOfOriginalInteger << std::dec << ", Value: " << originalInteger << std::endl;
    std::cout << std::endl;

    // Read original integer again and show its value
    readInteger = memoryCommando.ReadVirtualMemory<int>(addressOfOriginalInteger);
    std::cout << "readInteger: " << readInteger << std::endl;
    std::cout << std::endl;
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

![](https://i.imgur.com/HmfB3hz.png)

 
