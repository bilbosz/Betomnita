#pragma once
#include "project/Config.hpp"

#include <boost/lexical_cast.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace App
{
    class Error
    {
    public:
        static void Breakpoint();
        static void Message( const std::wstring& type, const std::string& file, uint32_t line, const std::wstring& message, bool abort );
    };
}

// clang-format off
#ifdef _DEBUG
#	define ASSERT(test,message) { if( !( test ) ) { std::wostringstream out; out << message; App::Error::Message( L"A", __FILE__, __LINE__, out.str(), true ); } }
#else
#	define ASSERT(test,message)
#endif
// clang-format on

// clang-format off
#ifdef _DEBUG
#	define MESSAGE(message) { std::wostringstream out; out << message; App::Error::Message( L"M", __FILE__, __LINE__, out.str(), false ); }
#else
#	define MESSAGE(message)
#endif
// clang-format on

// clang-format off
#ifdef _DEBUG
#	define VERIFY(test) { if( !( test ) ) App::Error::Breakpoint(); }
#else
#	define VERIFY(test) { ( test ); }
#endif
// clang-format on