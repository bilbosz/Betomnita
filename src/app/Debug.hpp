#pragma once
#ifdef DEBUG

#    include "project/Config.hpp"

#    include <boost/type_index.hpp>
#    include <iomanip>
#    include <iostream>
#    include <sstream>
#    include <string>

namespace App
{
    class Debug
    {
    public:
        static void Breakpoint();
        static void Message( const std::wstring& type, const std::string& file, uint32_t line, const std::wstring& message, bool abort );
        static void SetExpensive( bool value )
        {
            s_expensive = value;
        }
        static bool IsExpensive()
        {
            return s_expensive;
        }

    private:
        static bool s_expensive;
    };
}

#endif

// clang-format off
#ifdef DEBUG
#	define ASSERT(test,message) { if( !( test ) ) { std::wostringstream out; out << message; App::Debug::Message( L"A", __FILE__, __LINE__, out.str(), true ); } }
#else
#	define ASSERT(test,message)
#endif
// clang-format on

// clang-format off
#ifdef DEBUG
#	define CHECK(test) { if( !( test ) ) App::Debug::Breakpoint(); }
#else
#	define CHECK(test)
#endif
// clang-format on

// clang-format off
#ifdef DEBUG
#	define MESSAGE(message) { std::wostringstream out; out << message; App::Debug::Message( L"M", __FILE__, __LINE__, out.str(), false ); }
#else
#	define MESSAGE(message)
#endif
// clang-format on

// clang-format off
#ifdef DEBUG
#	define VERIFY(test) { if( !( test ) ) App::Debug::Breakpoint(); }
#else
#	define VERIFY(test) { ( test ); }
#endif
// clang-format on

// clang-format off
#ifdef DEBUG
#	define PRINT_TYPE(type) { std::wostringstream out; out << boost::typeindex::type_id_with_cvr< type >().pretty_name().c_str(); App::Debug::Message( L"T", __FILE__, __LINE__, out.str(), false ); }
#else
#	define PRINT_TYPE(type)
#endif
// clang-format on